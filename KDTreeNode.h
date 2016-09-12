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
KDTreeNode spKDTreeRecursion(SPKDArray kdarray, int i, SP_SPLIT_METHOD splitMethod);

/*
 *  Initialize the KDTree data structure
 *
 *  @param config - The configuration structure
 *  @param featuresArray - A pointer to the array that will store the features
 *  @param kdTree - The data structure we are initializing
 *
 *  @return True if the KDTree was successfully initialized
 *  		False if the initialization failed
 */
bool spKDTreeInit(SPConfig config, SPPoint* featuresArray, KDTreeNode kdTree);

/*
 * @@@@@@@@@@@ TODO
 *
 * @@@@@@@@@@@ TODO
 */
void destroyArray(KDTreeNode root);

#endif /* KDTREENODE_H_ */
