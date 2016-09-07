#ifndef KDTREENODE_H_
#define KDTREENODE_H_

#include "SPconfig.h"

typedef struct kd_tree_node_t *KDTreeNode;

/*
 * @@@@@@@@@@@ TODO
 *
 * @@@@@@@@@@@ TODO
 */
KDTreeNode createNode(int a);

/*
 * @@@@@@@@@@@ TODO
 *
 * @@@@@@@@@@@ TODO
 */
KDTreeNode* createFromArray(const SPConfig config, int* arr, int size);

/*
 * @@@@@@@@@@@ TODO
 *
 * @@@@@@@@@@@ TODO
 */
void destroyArray(KDTreeNode root);

#endif /* KDTREENODE_H_ */
