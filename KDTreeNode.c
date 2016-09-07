#include <stdlib.h>
#include "KDTreeNode.h"
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
KDTreeNode* createNode(int a) {
	KDTreeNode* node = malloc(sizeof(*node));
	node->dim = NULL;
	node->val = NULL;
	node->left = NULL;
	node->right = NULL;
	node->data = a;
	return node;
}

/* Given an array 'arr' the function returns a new
 * tree with the values of 'arr'*/
KDTreeNode* createFromArray(const SPConfig config, int* arr, int size) {
	int mid;
	SP_SPLIT_METHOD splitMethod;
	KDTreeNode* root;
	if (size == 0 || !arr) {
		root = NULL;
	} else if (size == 1) {
		root = createNode(arr[0]);
	} else {
		if (spConfigGetKDTreeSplitMethod(splitMethod, config) != SP_CONFIG_SUCCESS) {
			// TODO Error
		}
		switch (*splitMethod) {
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
void destroyArray(KDTreeNode* root) {
	if (!root) {
		return;
	}
	destroyArray(root->left);
	destroyArray(root->right);
	free(root);
}
