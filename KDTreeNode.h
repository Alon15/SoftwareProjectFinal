#ifndef KDTREENODE_H_
#define KDTREENODE_H_

#include "SPConfig.h"
#include "SPKDArray.h"
#include "SPPoint.h"

typedef struct kd_tree_node_t *KDTreeNode;

/*
 * @@@@@@@@@@@ TODO
 *
 * @@@@@@@@@@@ TODO
 */
KDTreeNode createKDTree(SPKDArray kdarray, int i, SP_SPLIT_METHOD splitMethod);

/*
 * @@@@@@@@@@@ TODO
 *
 * @@@@@@@@@@@ TODO
 */
KDTreeNode createFromArray(SPConfig config);

/*
 * @@@@@@@@@@@ TODO
 *
 * @@@@@@@@@@@ TODO
 */
void destroyArray(KDTreeNode root);

#endif /* KDTREENODE_H_ */
