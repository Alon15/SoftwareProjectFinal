#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPKDArray.h"
#include "SPPoint.h"
#include "sort_r.h"

// Array datatype
struct kd_array_t {
	int dim; // The dimension of each point (d)
	int size; // Points array size (n)
	SPPoint* points; // Array of points
	int** matrix; // Matrix for splitting purposes
};

//
int compare(const void *aIn, const void *bIn, void *thunkIn)
{
	const int *a = aIn, *b = bIn;
	const int *thunk = thunkIn;
	//float a   = *((float*)aIn); // TODO DEBUG DELME
	//float b   = *((float*)bIn); // TODO DEBUG DELME
	//int thunk = *((int*)thunkIn); // TODO DEBUG DELME
	if (thunk[*a] < thunk[*b]) {
		return -1;
	} else if (thunk[*a] > thunk[*b]) {
		return 1;
	} else {
		return 0;
	}
}

//
SPKDArray Init(SPPoint* arr, int size) {
	return InitFast(arr,size,NULL);
}

// Initializes the kd-array with the data given by arr.
SPKDArray InitFast(SPPoint* arr, int size, int** inptMtrx) {
	// Function variables
	int i,j; // Generic loop variable
	int dim; // Points array dimension
	SPKDArray KDarray;
	double **matrix_v; // Matrix of values
	int **matrix_i; // Matrix of index's
	// Allocate memory
	dim = spPointGetDimension(arr[0]);
	KDarray = (SPKDArray) malloc(sizeof(*KDarray));
	matrix_v = (double **)malloc(dim * sizeof(double)); // TODO dim = 2
	matrix_i = (int **)malloc(dim * sizeof(int*)); // TODO dim = 2
	if ((KDarray == NULL)||(matrix_v == NULL)||(matrix_i == NULL)) { // Memory allocation error
		return NULL;
	}
	KDarray->points = (SPPoint *) malloc(size * sizeof(*KDarray->points));
	if (KDarray->points == NULL) { // Memory allocation error
		free(KDarray);
		free(matrix_v);
		free(matrix_i);
		return NULL;
	}
	// TODO only if inptMtrx == NULL :
	for (i=0;i<dim;i++) { // TODO dim = 2
		matrix_v[i] = (double *)malloc(size * sizeof(double));
		matrix_i[i] = (int *)malloc(size * sizeof(int));
		if ((matrix_v[i] == NULL)||(matrix_i[i] == NULL)) { // Memory allocation error
			free(KDarray->points);
			free(KDarray);
			free(matrix_v);
			free(matrix_i);
			return NULL;
		}
	}
	// Function body
	KDarray->dim = dim; // TODO dim = 2
	KDarray->size = size;
	for (i=0;i<size;i++) {
		KDarray->points[i] = spPointCopy(arr[i]); // The example from FinalProject.pdf (page 10) will look like:
		for (j=0;j<dim;j++) { //										|   1 | 123 |   2 |   9 |   3 |
			matrix_v[j][i] = spPointGetAxisCoor(arr[i],j); //matrix_v = |   2 |  70 |   7 |  11 |   4 |
			matrix_i[j][i] = i; //										|   0 |   1 |   2 |   3 |   4 |
		} //												 matrix_i = |   0 |   1 |   2 |   3 |   4 |
	}
	if (inptMtrx == NULL) {
		for (i=0;i<dim;i++) { // Foreach dim// TODO dim = 2
			qqsort(matrix_i[i],size,sizeof(matrix_i[i][0]),compare,matrix_v[i]);
		}
	} else {
		matrix_i = NULL;//inptMtrx; // TODO fix this line
	}
	KDarray->matrix = matrix_i;
	// Free memory
    for(i=0;i<size;i++) {
    	//for(j=0;j<dim;j++) { // TODO dim = 2
        //	if (matrix_v[i][j]) { // A tiny chance for errors in some compilers
        //        free(matrix_v[i][j]);
        //	}
    	//}
        if (matrix_v[i]) { // A tiny chance for errors in some compilers
        	free(matrix_v[i]);
        }
    }
    if (matrix_v) { // A tiny chance for errors in some compilers
    	free(matrix_v);
    	matrix_v = NULL; // Preventing a "double-free"
    }
	// Finish
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
	if (coor == 0) { // TODO fix this line
		KDarrayLeft = InitFast(kdArr,(kdArr->size)/2,kdArr->matrix); // TODO fix this line
		KDarrayRight = InitFast(kdArr,(kdArr->size)-((kdArr->size)/2),kdArr->matrix); // TODO fix this line
	}
	return NULL; // TODO fix this line
}
