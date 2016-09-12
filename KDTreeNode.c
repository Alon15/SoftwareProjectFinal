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
	KDTreeNode node;
	SPKDArrayPair nodeSons;
	// Allocate memory
	node = (KDTreeNode) malloc(sizeof(*node));
	nodeSons = (SPKDArrayPair) malloc(sizeof(*nodeSons));
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
			case RANDOM:
				i = 0; // TODO
				break;
			case MAX_SPREAD:
				i = 0; // TODO
				break;
			case INCREMENTAL:
				i = 0; // TODO
				break;
			default: // Just in case
				free(node);
				free(nodeSons);
				return NULL;
		}
		nodeSons = spKDArraySplit(kdarray,i); // Split by the i dimension
		node->dim = 0; // TODO
		node->val = 0; // TODO
		node->left = spKDTreeRecursion(spKDArrayPairGetLeft(nodeSons),i+1,splitMethod); // TODO
		node->right = spKDTreeRecursion(spKDArrayPairGetRight(nodeSons),i+1,splitMethod); // TODO
		node->data = NULL;
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
	return true;
}

// Frees all allocation associated with the tree given by root
void spKDTreeDestroy(KDTreeNode root) {
	if (!root) {
		return;
	}
	destroyArray(root->left);
	destroyArray(root->right);
	free(root);
}
