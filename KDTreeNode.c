#include <stdlib.h>
#include "KDTreeNode.h"
#include "SPConfig.h"
#include "SPKDArray.h"

// Tree datatype
struct kd_tree_node_t {
	int dim; // The splitting dimension
	int val; // The median value of the splitting dimension
	struct kd_tree_node_t* left; // Pointer to the left subtree
	struct kd_tree_node_t* right; // Pointer to the right subtree
	int data; // Pointer to a point (only if the current node is a leaf) otherwise this field value is NULL
};

// creates one node tree with the value 'a'
KDTreeNode createKDNode(int a) {
	// Function variables
	KDTreeNode node;
	// Allocate memory
	node = (KDTreeNode) malloc(sizeof(*node));
	if (node == NULL) { // Memory allocation error
		return NULL;
	}
	// Function body
	node->dim = 0;
	node->val = 0;
	node->left = NULL;
	node->right = NULL;
	node->data = a;
	return node;
}

/* Given an array 'arr' the function returns a new
 * tree with the values of 'arr'*/
KDTreeNode* createFromArray(SPConfig config, int* arr, int size) {
	// Function variables
	int mid;
	SP_CONFIG_MSG config_msg;
	SP_SPLIT_METHOD splitMethod;
	KDTreeNode* root;
	// Allocate memory
	config_msg = SP_CONFIG_SUCCESS;
	root = (KDTreeNode) malloc(sizeof(*root));
	if (root == NULL) { // Memory allocation error
			return NULL;
	}
	// Function body
	if (size == 0 || !arr) {
		root = NULL;
	} else if (size == 1) {
		root = createNode(arr[0]);
	} else {
		config_msg = spConfigGetKDTreeSplitMethod(&splitMethod,config);
		if (config_msg != SP_CONFIG_SUCCESS) {
			//TODO error
		}
		switch (splitMethod) {
			case RANDOM:

				break;
			case MAX_SPREAD:

				break;
			case INCREMENTAL:

				break;
		}
	}
	//mid = size / 2;
	//KDTreeNode* left = createFromArray(arr, size / 2);
	//KDTreeNode* right = createFromArray(arr + size / 2 + 1, size - size / 2 - 1);
	//KDTreeNode* root = createNode(arr[mid]);
	//root->left = left;
	//root->right = right;
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
