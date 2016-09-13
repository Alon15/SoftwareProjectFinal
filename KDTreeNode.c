#include <stdlib.h>
#include "KDTreeNode.h"
#include "SPConfig.h"
#include "SPKDArray.h"
#include "SPPoint.h"

// Tree datatype
struct kd_tree_node_t {
	int dim; // The splitting dimension
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
		}
		nodeSons = spKDArraySplit(kdarray,i); // Split by the i dimension
		node->dim = i;
		node->val = 0; // The median according the dimension i // TODO
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
	kdTree = spKDTreeRecursion(kdArray,0,splitMethod); // TODO
	if (kdTree == NULL) {
		return false;
	} else {
		return true;
	}
}

// Frees all allocation associated with the tree given by root
void spKDTreeDestroy(KDTreeNode root) {
	if (!root) {
		return;
	}
	spKDTreeDestroy(root->left);
	spKDTreeDestroy(root->right);
	free(root);
}
