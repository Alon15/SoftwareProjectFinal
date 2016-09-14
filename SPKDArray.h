#ifndef SPKDARRAY_H_
#define SPKDARRAY_H_

#include "SPPoint.h"

typedef struct kd_array_t *SPKDArray;

typedef struct kd_array_pair_t *SPKDArrayPair;

/*
 * A Comparator function for the qsort function.
 *
 * The function compare doubles
 *
 * @param aIn - The index of the first element
 * @param bIn - The index of the second element
 * @param thunkIn - The array of the elements we are comparing
 *
 * @return 1 If thunk[aIn] > thunk[bIn]
 * 		  -1 If thunk[aIn] < thunk[bIn]
 * 		   0 If thunk[aIn] = thunk[bIn]
 */
int spKDArrayCompare(const void *aIn, const void *bIn, const void *thunkIn);

/*
 * Initializes the kd-array with the data given by arr. The complexity of
 * this operation is O(d X nlog(n))
 *
 * @param arr - Array of the features
 * @param size - The size of the array (number of features)
 *
 * @return The kd array on success
 * 		   NULL on failure
 */
SPKDArray spKDArrayInit(SPPoint* arr, int size);

/*
 * Returns two kd-arrays (kdLeft, kdRight) such that the first [n/2] points
 * with respect to the coordinate coor are in kdLeft , and the rest of the
 * points are in kdRight.
 *
 * @param kdArr - The KDArray we are splitting
 * @param coor - The coordinate we are splitting wit respect to
 *
 * @return a structure that hold the 2 new KDArrays
 */
SPKDArrayPair spKDArraySplit(SPKDArray kdArr, int coor);

/*
 * Free all the memory associated with the KDArray
 *
 * @param array - The array we are freeing
 *
 */
void spKDArrayDestroy(SPKDArray array);

/*
 * Getters function
*/

int spKDArrayGetDimension(SPKDArray array);

int spKDArrayGetSize(SPKDArray array);

double* spKDArrayGetMinSpread(SPKDArray array);

double* spKDArrayGetMaxSpread(SPKDArray array);

SPPoint* spKDArrayGetPoints(SPKDArray array);

int** spKDArrayGetMatrix(SPKDArray array);

SPKDArray spKDArrayPairGetLeft(SPKDArrayPair array);

SPKDArray spKDArrayPairGetRight(SPKDArrayPair array);

#endif /* SPKDARRAY_H_ */
