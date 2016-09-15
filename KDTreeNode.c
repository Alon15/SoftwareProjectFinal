#include <stdlib.h> // malloc, calloc, free, NULL, rand
#include <time.h> // time
#include "KDTreeNode.h"
#include "SPConfig.h"
#include "SPKDArray.h"
#include "SPPoint.h"
#include "defines.h"
#include "SPLogger.h"
#include "SPBPriorityQueue.h"
#include "SPListElement.h"

// Tree datatype
struct kd_tree_node_t {
	int dim; // The splitting dimension
	double val; // The median value of the splitting dimension
	struct kd_tree_node_t* left; // Pointer to the left subtree
	struct kd_tree_node_t* right; // Pointer to the right subtree
	SPPoint data; // Pointer to a point (only if the current node is a leaf) otherwise this field value is NULL
};

// Recursively creates KD tree from KD array
// TODO massive memory leak in this function
KDTreeNode spKDTreeRecursion(SPKDArray kdarray, int i, SP_SPLIT_METHOD splitMethod) {
	// Function variables
	double tmpLoopVar;
	int j; // Generic loop variable
	int tmpVar1,tmpVar2;
	KDTreeNode node;
	SPKDArrayPair nodeSons;
	double* minSpreadArray;
	double* maxSpreadArray;
	// Allocate memory
	node = (KDTreeNode)malloc(sizeof(*node));
	// Function body
	if ((node == NULL)||(spKDArrayGetSize(kdarray) == 0)) {
		/*if (node) {
			free(node);
		}*/
		/*if (nodeSons) { // TODO DELME
			free(nodeSons); // TODO DELME
		}
		if (minSpreadArray) { // TODO DELME
			free(minSpreadArray); // TODO DELME
		}
		if (maxSpreadArray) { // TODO DELME
			free(maxSpreadArray); // TODO DELME
		}*/
		node->dim = -1;
		return node;
	} else if (spKDArrayGetSize(kdarray) == 1) {
		node->dim = 0;
		node->val = 0;
		node->left = NULL;
		node->right = NULL;
		node->data = spKDArrayGetPoints(kdarray)[0]; // Does not duplicate the point!!!
	} else {
		switch (splitMethod) {
			case RANDOM: // TODO verify
				i = rand() % spKDArrayGetDimension(kdarray);
				break;
			case MAX_SPREAD: // TODO verify
				i = 0;
				minSpreadArray = spKDArrayGetMinSpread(kdarray);
				maxSpreadArray = spKDArrayGetMaxSpread(kdarray);
				tmpLoopVar = 0;
				for (j=0;j<spKDArrayGetDimension(kdarray);j++) {
					if (maxSpreadArray[j]-minSpreadArray[j] > tmpLoopVar) { // If dim j spread different is the biggest so far
						tmpLoopVar = maxSpreadArray[j]-minSpreadArray[j]; // Update what was the maximum wee saw
						i = j; // Update the splitting dimension
					}
				}
				break;
			case INCREMENTAL: // TODO verify
				i = (i+1) % spKDArrayGetDimension(kdarray);
				break;
			default: // Just in case
				node->dim = -1;
				return node;
		} // Find the index of the middle point in the sorted array, And then get the selected point value for the given coordinate
		if (spKDArrayGetSize(kdarray)%2 == 0) { // Size is even, Choose the AVG of the two points that in the middle
			tmpVar1 = spPointGetAxisCoor(spKDArrayGetPoints(kdarray)[spKDArrayGetMatrix(kdarray)[i][(spKDArrayGetSize(kdarray)/2)-1]],i);
			tmpVar2 = spPointGetAxisCoor(spKDArrayGetPoints(kdarray)[spKDArrayGetMatrix(kdarray)[i][spKDArrayGetSize(kdarray)/2]],i);
			node->val = (tmpVar1+tmpVar2)/2;
		} else { // Size is odd, Choose the middle
			node->val = spPointGetAxisCoor(spKDArrayGetPoints(kdarray)[spKDArrayGetMatrix(kdarray)[i][spKDArrayGetSize(kdarray)%2]],i);
		}
		nodeSons = spKDArraySplit(kdarray,i); // Split by the i dimension
		node->dim = i;
		node->left = spKDTreeRecursion(spKDArrayPairGetLeft(nodeSons),i,splitMethod);
		node->right = spKDTreeRecursion(spKDArrayPairGetRight(nodeSons),i,splitMethod);
		node->data = NULL;
	}
	// Free memory
	/*if (nodeSons) { // TODO "No source available for ntdll!RtlFreeHeap" error
		spKDArrayPairDestroy(nodeSons);
	}
	if (minSpreadArray) { // A tiny chance for errors in some compilers
		free(minSpreadArray);
		minSpreadArray = NULL; // Preventing a "double-free"
	}
	if (maxSpreadArray) { // A tiny chance for errors in some compilers
		free(maxSpreadArray);
		maxSpreadArray = NULL; // Preventing a "double-free"
	}
	spKDArrayDestroy(kdarray);*/
	// Finish
	if (((node->left)&&(node->left->dim == -1))||((node->right)&&(node->right->dim == -1))) { // Bubble the alert back to the root
		node->dim = -1;
	}
	return node;
}

bool spKDTreeInit(SPConfig config, SPPoint* featuresArray, int size, KDTreeNode* kdTree) {
	// Function variables
	int i; // Generic loop variable
	SP_CONFIG_MSG config_msg;
	SP_SPLIT_METHOD splitMethod;
	SPKDArray kdArray;
	// Allocate memory
	config_msg = SP_CONFIG_SUCCESS;
	*kdTree = (KDTreeNode)malloc(sizeof(*kdTree));
	*kdTree = NULL;
	if (kdTree == NULL) {
		return false;
	}
	// Function body
	config_msg = spConfigGetKDTreeSplitMethod(&splitMethod,config);
	if (config_msg != SP_CONFIG_SUCCESS) {
		return false;
	}
	kdArray = spKDArrayInit(featuresArray,size);
	if (kdArray == NULL) {
		return false;
	}
	srand((unsigned int)time(NULL));
	*kdTree = spKDTreeRecursion(kdArray,-1,splitMethod);
	// Free memory
	if (featuresArray) { // A tiny chance for errors in some compilers
		for (i=0;i<size;i++) {
			spPointDestroy(featuresArray[i]);
			featuresArray[i] = NULL; // Preventing a "double-free"
		}
		free(featuresArray);
		featuresArray = NULL; // Preventing a "double-free"
	}
	// Finish
	if ((*kdTree == NULL)||((*kdTree)->dim == -1)) {
		spKDTreeDestroy(*kdTree);
		return false;
	} else {
		return true;
	}
}
/*
 * Recursive function that find the k nearest features to our feature and store them in the Priority Queue
 *
 * @param kdTree - The current node we are checking
 * @param bpq - The priority queue we are filling
 * @param feature - The query feature, we are searching the k closest features to him
 *
 * @return False if a memory allocation error occurred
 * 		   True otherwise
 */
bool recKNNSearch(KDTreeNode kdTree,SPBPQueue bpq,SPPoint feature) {
	// Function variables
	SPListElement element;
	bool leftSide = false;
	double diff;
	// Function body
	if (kdTree == NULL) { // subtree is NULL
		return true;
	} else if (kdTree->data != NULL) { // kdTree is a leaf
		element = spListElementCreate(spPointGetIndex(kdTree->data),spPointL2SquaredDistance(kdTree->data,feature));
		if (element == NULL) {
			PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		}
		if (spBPQueueEnqueue(bpq,element) == SP_BPQUEUE_OUT_OF_MEMORY) {
			PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		}
		spListElementDestroy(element);
		return true;
	} else if (spPointGetAxisCoor(feature,kdTree->dim) <= kdTree->val) {
		if (!recKNNSearch(kdTree->left,bpq,feature)) { // recursively search the left subtree
			return false;
		}
		leftSide = true;
	} else {
		if (!recKNNSearch(kdTree->right,bpq,feature)) { // recursively search the right subtree
			return false;
		}
	}
	// check if the candidate hypersphere crosses the splitting plane
	diff = (kdTree->val)-spPointGetAxisCoor(feature,kdTree->dim);
	if (!spBPQueueIsFull(bpq) || (diff*diff) < spBPQueueMaxValue(bpq)) {
		if (leftSide) {
			if (!recKNNSearch(kdTree->right,bpq,feature)) { // recursively search the right subtree
				return false;
			}
		} else {
			if (!recKNNSearch(kdTree->left,bpq,feature)) { // recursively search the left subtree
				return false;
			}
		}
	}
	return true;
}

int* kNearestNeighborsSearch(SPConfig config, KDTreeNode kdTree, SPPoint feature) {
	// Function variables
	SPBPQueue bpq;
	SP_CONFIG_MSG config_msg;
	int KNN, i;
	int* NNArray;
	SPListElement element;
	// Function body
	if (kdTree == NULL) {
		PRINT_ERROR_LOGGER(KDTREE_IS_NULL,__FILE__,__func__,__LINE__);
		return NULL;
	}
	KNN = spConfigGetKNN(config,&config_msg);
	if (config_msg != SP_CONFIG_SUCCESS) {
		PRINT_ERROR_LOGGER(GET_KNN_FAIL_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	bpq = spBPQueueCreate(KNN);
	if (bpq == NULL) {
		PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	if (!recKNNSearch(kdTree,bpq,feature)) {
		PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		spBPQueueDestroy(bpq);
		return NULL;
	}
	NNArray = (int*)malloc(sizeof(int)*KNN);
	if (NNArray == NULL) {
		PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		spBPQueueDestroy(bpq);
		return NULL;
	}
	for (i=0;i<KNN;i++) {
		if (spBPQueueIsEmpty(bpq)) {
			PRINT_ERROR_LOGGER(BPQ_EMPTY_ERROR,__FILE__,__func__,__LINE__);
			free(NNArray);
			spBPQueueDestroy(bpq);
			return NULL;
		}
		element = spBPQueuePeek(bpq);
		if (element == NULL) {
			PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
			free(NNArray);
			spBPQueueDestroy(bpq);
			return NULL;
		}
		NNArray[i] = spListElementGetIndex(element);
		spListElementDestroy(element);
		if (spBPQueueDequeue(bpq)!= SP_BPQUEUE_SUCCESS) {
			free(NNArray);
			spBPQueueDestroy(bpq);
			PRINT_ERROR_LOGGER(BPQ_EMPTY_ERROR,__FILE__,__func__,__LINE__);
			return NULL;
		}
	}
	spBPQueueDestroy(bpq);
	return NNArray;
}

int* closestImagesQuery(SPConfig config, KDTreeNode kdTree, SPPoint* queryArray, int numOfFeat) {
	// Function variables
	int i,j; // Generic loop variables
	int max, numOfSimilarImages, numOfImages;
	int* imageHitsArray, *bestMatches, *closestImages;
	SP_CONFIG_MSG config_msg;
	int KNN;
	// Allocate memory
	numOfImages = spConfigGetNumOfImages(config,&config_msg);
	if (config_msg != SP_CONFIG_SUCCESS) {
		PRINT_ERROR_LOGGER(GET_NUM_OF_IMAGES_FAIL_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	numOfSimilarImages = spConfigGetNumOfSimilarImages(config,&config_msg);
	if (config_msg != SP_CONFIG_SUCCESS) {
		PRINT_ERROR_LOGGER(GET_NUM_OF_IMAGES_FAIL_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	KNN = spConfigGetKNN(config,&config_msg);
	if (config_msg != SP_CONFIG_SUCCESS) {
		return NULL;
	}
	imageHitsArray = (int*)calloc(numOfImages,sizeof(int)); // Feature's hit counter
	closestImages = (int*)malloc(sizeof(int)*numOfSimilarImages); // Store the results
	if (imageHitsArray == NULL || closestImages == NULL) { // Memory allocation error
		if(imageHitsArray) {
			free(imageHitsArray);
		}
		if(closestImages) {
			free(closestImages);
		}
		PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	// Function code
	for (i=0;i<numOfFeat;i++) { // For each feature find the closest features's image index
		bestMatches = kNearestNeighborsSearch(config,kdTree,queryArray[i]);
		if(!bestMatches) {
			free(imageHitsArray);
			free(closestImages);
			PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
			return NULL;
		}
		for (j=0;j<KNN;j++) {
			imageHitsArray[bestMatches[j]]++; // Update the hit counter
		}
	}
	// find the index of the images with the most hits
	for (i=0;i<numOfSimilarImages;i++) {
		max = 0;
		for (j=0;j<numOfImages;j++) {
			if (imageHitsArray[j] > max) {
				max = imageHitsArray[j];
				closestImages[i] = j; // set the new highest hit count index
			}
		}
		imageHitsArray[closestImages[i]] = -1; // 'remove' the maximum from the search
	}
	free(imageHitsArray);
	free(bestMatches);
	return closestImages;
}

// Frees all allocation associated with the tree given by root
void spKDTreeDestroy(KDTreeNode root) {
	if (root != NULL) {
		if (root->left) {
			spKDTreeDestroy(root->left);
		}
		if (root->right) {
			spKDTreeDestroy(root->right);
		}
		if (root->data) {
			spPointDestroy(root->data);
		}
		free(root);
	}
}
