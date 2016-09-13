#ifndef SPKDARRAY_H_
#define SPKDARRAY_H_

#include "SPPoint.h"

typedef struct kd_array_t *SPKDArray;

typedef struct kd_array_pair_t *SPKDArrayPair;

/*
 * TODO
 *
 * @param TODO
 * @return TODO
 */
int spKDArrayCompare(const void *aIn, const void *bIn, void *thunkIn);

/*
 * Initializes the kd-array with the data given by arr. The complexity of
 * this operation is O(d X nlog(n))
 *
 * @param TODO
 * @return TODO
 */
SPKDArray spKDArrayInit(SPPoint* arr, int size);

/*
 * Returns two kd-arrays (kdLeft, kdRight) such that the first [n/2] points
 * with respect to the coordinate coor are in kdLeft , and the rest of the
 * points are in kdRight.
 *
 * @param TODO
 * @return TODO
 */
SPKDArrayPair spKDArraySplit(SPKDArray kdArr, int coor);

void spKDArrayDestroy(SPKDArray array);

int spKDArrayGetDimension(SPKDArray array);

int spKDArrayGetSize(SPKDArray array);

double* spKDArrayGetMinSpread(SPKDArray array);

double* spKDArrayGetMaxSpread(SPKDArray array);

SPPoint* spKDArrayGetPoints(SPKDArray array);

int** spKDArrayGetMatrix(SPKDArray array);

SPKDArray spKDArrayPairGetLeft(SPKDArrayPair array);

SPKDArray spKDArrayPairGetRight(SPKDArrayPair array);

#endif /* SPKDARRAY_H_ */
