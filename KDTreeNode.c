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
KDTreeNode createKDTree(SPKDArray kdarray, int i, SP_SPLIT_METHOD splitMethod) {
	// Function variables
	KDTreeNode node;
	// Allocate memory
	node = (KDTreeNode) malloc(sizeof(*node));
	if (node == NULL) { // Memory allocation error
		return NULL;
	}
	// Function body
	if (spKDArrayGetSize(kdarray) == 0) {
		free(node);
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

				break;
			case MAX_SPREAD:

				break;
			case INCREMENTAL:

				break;
			default: // Just in case
				free(node);
				return NULL;
		}
		node->dim = 0; // TODO
		node->val = 0; // TODO
		node->left = createKDTree(kdarray,i+1,splitMethod); // TODO
		node->right = createKDTree(kdarray,i+1,splitMethod); // TODO
		node->data = NULL;
		if ((node->left == NULL)||(node->right == NULL)) { // Bubble the alert back to the root
			free(node);
			return NULL;
		}
	}
	return node;
}

//
KDTreeNode createFromArray(SPConfig config) {
	// Function variables
	SP_CONFIG_MSG config_msg;
	SP_SPLIT_METHOD splitMethod;
	KDTreeNode root;
	// Allocate memory
	config_msg = SP_CONFIG_SUCCESS;
	//root = (KDTreeNode) malloc(sizeof(*root));
	//if (root == NULL) { // Memory allocation error
	//		return NULL;
	//}
	// Function body
	config_msg = spConfigGetKDTreeSplitMethod(&splitMethod,config);
	if (config_msg != SP_CONFIG_SUCCESS) {
		//TODO error
	}
	// TODO Create KD array from features
	root = createKDTree(NULL,0,splitMethod); // TODO
	return root;
}

// Frees all allocation associated with the tree given by root
void destroyArray(KDTreeNode root) {
	if (!root) {
		return;
	}
	destroyArray(root->left);
	destroyArray(root->right);
	free(root);
}
