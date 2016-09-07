#include <stdlib.h>
#include "SPKDArray.h"
#include "SPPoint.h"

// Array datatype
struct kd_array_t {
	int dim; // The dimension of each point (d)
	int size; // Points array size (n)
	SPPoint* points; // Array of points
	int** matrix; // Matrix for splitting purposes
};

// Initializes the kd-array with the data given by arr.
SPKDArray Init(SPPoint* arr, int size, int** inptMtrx = NULL) {
	// Function variables
	int i,j; // Generic loop variable
	SPKDArray KDarray;
	int **matrix;
	// Allocate memory
	KDarray = (SPKDArray) malloc(sizeof(*KDarray));
	matrix = (int **)malloc(2 * sizeof(int*)); // TODO dim = 2
	if ((KDarray == NULL)||(matrix == NULL)) { // Memory allocation error
		return NULL;
	}
	KDarray->points = (SPPoint) malloc(sizeof(*KDarray->points));
	if (KDarray->points == NULL) { // Memory allocation error
		free(KDarray);
		return NULL;
	}
	// TODO only if inptMtrx == NULL :
	for (i=0;i<2;i++) { // TODO dim = 2
		matrix[i] = (int *)malloc(size * sizeof(int));
		if (matrix[i] == NULL) { // Memory allocation error
			free(KDarray->points);
			free(KDarray);
			return NULL;
		}
	}
	// Function body
	KDarray->dim = 2; // TODO dim = 2
	KDarray->size = size;
	KDarray->points = arr;
	if (inptMtrx == NULL) {
		for (i=0;i<2;i++) { // Foreach dim// TODO dim = 2
			for (j=0;j<size;j++) { // TODO DEBUG DELME
				matrix[i][j] = j; // TODO DEBUG DELME
			} // TODO DEBUG DELME
		}
	} else {
		matrix = inptMtrx; // TODO fix this line
	}
	KDarray->matrix = matrix;
	return KDarray;
}

//
SPKDArray* Split(SPKDArray kdArr, int coor) {
	// Function variables
	SPKDArray KDarrayLeft;
	SPKDArray KDarrayRight;
	// Allocate memory
	KDarrayLeft = (SPKDArray) malloc(sizeof(*KDarrayLeft));
	KDarrayRight = (SPKDArray) malloc(sizeof(*KDarrayRight));
	// Function body
	KDarrayLeft = Init(kdArr,(kdArr->size)/2,kdArr->matrix); // TODO fix this line
	KDarrayRight = Init(kdArr,(kdArr->size)-((kdArr->size)/2),kdArr->matrix); // TODO fix this line
	return NULL; // TODO fix this line
}
