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
bool spKDTreeInit(SPConfig config, SPPoint* featuresArray, int size, KDTreeNode* kdTree);

/*
 * Find the k nearest features in our kdTree to the query feature
 *
 * @param config - The configuration structure
 * @param kdTree - The data structure of our features
 * @param feature - The query feature
 *
 * @return A pointer to the array of the indexes of the k nearest features
 * 		   NULL if an error occurred
 */
int* kNearestNeighborsSearch(SPConfig config, KDTreeNode kdTree, SPPoint feature);

/*
 * Find the closest images to the query image
 *
 * @param config - The configuration structure
 * @param kdTree - The data structure
 * @param queryArray - Array of the features of the query image
 * @param numOfFeat - The number of features in the queryArray
 *
 * @return Pointer to the array that contain the indexes of the closest images
 * 		   The size of the array is detriment by the 'numOfSimilarImages' field in the config
 */
int* closestImagesQuery(SPConfig config, KDTreeNode kdTree, SPPoint* queryArray, int numOfFeat);

/*
 * Free all the memory associated with the kdTree
 *
 * @param root - The root of the kdTree we are freeing
 *
 */
void spKDTreeDestroy(KDTreeNode root);

#endif /* KDTREENODE_H_ */
