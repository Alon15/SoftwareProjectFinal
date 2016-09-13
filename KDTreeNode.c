#include <stdlib.h> // malloc, calloc, free, NULL, rand
#include "KDTreeNode.h"
#include "SPConfig.h"
#include "SPKDArray.h"
#include "SPPoint.h"
#include "defines.h"
#include "SPBPriorityQueue.h"

// Tree datatype
struct kd_tree_node_t {
	double dim; // The splitting dimension
	int val; // The median value of the splitting dimension
	struct kd_tree_node_t* left; // Pointer to the left subtree
	struct kd_tree_node_t* right; // Pointer to the right subtree
	SPPoint data; // Pointer to a point (only if the current node is a leaf) otherwise this field value is NULL
};

// Recursively creates KD tree from KD array
KDTreeNode spKDTreeRecursion(SPKDArray kdarray, int i, SP_SPLIT_METHOD splitMethod) {
	// Function variables
	double tmpLoopVar;
	int j;
	int tmpVar1,tmpVar2;
	KDTreeNode node;
	SPKDArrayPair nodeSons;
	double* minSpreadArray;
	double* maxSpreadArray;
	// Allocate memory
	node = (KDTreeNode) malloc(sizeof(*node));
	nodeSons = (SPKDArrayPair) malloc(sizeof(SPKDArrayPair));
	if ((node == NULL)||(nodeSons == NULL)) { // Memory allocation error
		free(node);
		free(nodeSons);
		return NULL;
	}
	// Function body
	if (spKDArrayGetSize(kdarray) == 0) {
		free(node);
		free(nodeSons);
		return NULL;
	} else if (spKDArrayGetSize(kdarray) == 1) {
		node->dim = 0;
		node->val = 0;
		node->left = NULL;
		node->right = NULL;
		node->data = spKDArrayGetPoints(kdarray)[0];
	} else {
		switch (splitMethod) {
			case RANDOM: // TODO verify
				i = rand() % (spKDArrayGetDimension(kdarray) + 1);
				break;
			case MAX_SPREAD: // TODO verify
				i = 0;
				minSpreadArray = spKDArrayGetMinSpread(kdarray);
				maxSpreadArray = spKDArrayGetMaxSpread(kdarray);
				for (j=0;j<spKDArrayGetDimension(kdarray);j++) {
					tmpLoopVar = 0;
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
				free(node);
				free(nodeSons);
				return NULL;
		} // Find the index of the middle point in the sorted array, And then get the selected point value for the given coordinate
		if (spKDArrayGetSize(kdarray)%2 == 0) { // Size is even, Choose the AVG of the two points that in the middle
			tmpVar1 = spPointGetAxisCoor(spKDArrayGetPoints(kdarray)[spKDArrayGetMatrix(kdarray)[i][(spKDArrayGetSize(kdarray)%2)-1]],i);
			tmpVar2 = spPointGetAxisCoor(spKDArrayGetPoints(kdarray)[spKDArrayGetMatrix(kdarray)[i][spKDArrayGetSize(kdarray)%2]],i);
			node->val = (tmpVar1+tmpVar2)/2;
		} else { // Size is odd, Choose the middle
			node->val = spPointGetAxisCoor(spKDArrayGetPoints(kdarray)[spKDArrayGetMatrix(kdarray)[i][spKDArrayGetSize(kdarray)%2]],i);
		}
		nodeSons = spKDArraySplit(kdarray,i); // Split by the i dimension
		node->dim = i;
		node->left = spKDTreeRecursion(spKDArrayPairGetLeft(nodeSons),i,splitMethod); // TODO verify
		node->right = spKDTreeRecursion(spKDArrayPairGetRight(nodeSons),i,splitMethod); // TODO verify
		node->data = NULL;
		free(nodeSons);
		if ((node->left == NULL)||(node->right == NULL)) { // Bubble the alert back to the root
			free(node);
			return NULL;
		}
	}
	return node;
}

bool spKDTreeInit(SPConfig config, SPPoint* featuresArray, int size, KDTreeNode kdTree) {
	// Function variables
	SP_CONFIG_MSG config_msg;
	SP_SPLIT_METHOD splitMethod;
	SPKDArray kdArray;
	// Allocate memory
	kdTree = NULL;
	config_msg = SP_CONFIG_SUCCESS;
	// Function body
	config_msg = spConfigGetKDTreeSplitMethod(&splitMethod,config);
	if (config_msg != SP_CONFIG_SUCCESS) {
		return false;
	}
	kdArray = spKDArrayInit(featuresArray,size);
	if (kdArray == NULL) {
		return false;
	}
	kdTree = spKDTreeRecursion(kdArray,-1,splitMethod);
	if (kdTree == NULL) {
		return false;
	} else {
		return true;
	}
}

int* kNearestNeighborsSearch(SPConfig config, KDTreeNode kdTree, SPPoint feature) {
	// Function variables
	SPBPQueue bpq;
	SP_BPQUEUE_MSG bpq_msg;
	// Function body
	// TODO implement the KNN search. page 14 in the pdf
	if (kdTree == NULL) {
		return NULL;
	} else if (kdTree->data != NULL) { // kdTree is a leaf
		//enqueue (index(current), distance(curr,p)) into bpq
		return NULL;
	} else if (spPointGetAxisCoor(feature,kdTree->dim) <= kdTree->val) {
		// Recursively search the left subtree
	} else {
		// Recursively search the right subtree
	}
	return NULL;
}

int* closestImagesQuery(SPConfig config, KDTreeNode kdTree, SPPoint* queryArray, int numOfFeat) {
	// Function variables
	int i,j; // Generic loop variables
	int max, numOfSimilarImages, numOfImages;
	int* imageHitsArray, *bestMatches, *closestImages;
	SP_CONFIG_MSG config_msg;
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
	imageHitsArray = (int*)calloc(numOfImages,sizeof(int)); // Feature's hit counter
	closestImages = (int*)malloc(sizeof(int)*numOfSimilarImages); // Store the results
	if (imageHitsArray == NULL || closestImages == NULL) { // Memory allocation error
		if(imageHitsArray)
			free(imageHitsArray);
		if(closestImages)
			free(closestImages);
		PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	// Function code
	for (i=0;i<numOfFeat;i++) { // For each feature find the closest features's image index
		bestMatches = kNearestNeighborsSearch(config,kdTree,queryArray[i]);
		if(!bestMatches){
			free(imageHitsArray);
			free(closestImages);
			return NULL;
		}
		for (j=0;j<numOfSimilarImages;j++) {
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
	return closestImages;
}

// Frees all allocation associated with the tree given by root
void spKDTreeDestroy(KDTreeNode root) {
	if (!root) {
		return;
	}
	spKDTreeDestroy(root->left);
	spKDTreeDestroy(root->right);
	//TODO need to free SPPoint data too?
	free(root);
}
