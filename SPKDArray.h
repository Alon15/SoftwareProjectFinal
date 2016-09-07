#ifndef SPKDARRAY_H_
#define SPKDARRAY_H_

/*
 * Initializes the kd-array with the data given by arr. The complexity of
 * this operation is O(d X nlog(n))
 *
 * @param TODO
 * @return TODO
 */
Init(SPPoint* arr, int size);

/*
 * Returns two kd-arrays (kdLeft, kdRight) such that the first [n/2] points
 * with respect to the coordinate coor are in kdLeft , and the rest of the
 * points are in kdRight.
 *
 * @param TODO
 * @return TODO
 */
Split(SPKDArray kdArr, int coor);

#endif /* SPKDARRAY_H_ */
