#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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

struct kd_array_pair_t {
	SPKDArray left;
	SPKDArray right;
};

//
int spKDArrayCompare(const void *aIn, const void *bIn, void *thunkIn)
{
	const int *a = aIn, *b = bIn;
	const double *thunk = thunkIn;
	if (thunk[*a] < thunk[*b]) {
		return -1;
	} else if (thunk[*a] > thunk[*b]) {
		return 1;
	} else {
		return 0;
	}
}

// Initializes the kd-array with the data given by arr.
SPKDArray spKDArrayInit(SPPoint* arr, int size) {
	// Function variables
	int i,j; // Generic loop variable
	int dim; // Points array dimension
	SPKDArray KDarray;
	double **matrix_v; // Matrix of values
	int **matrix_i; // Matrix of index's
	// Allocate memory
	dim = spPointGetDimension(arr[0]);
	KDarray = (SPKDArray) malloc(sizeof(*KDarray));
	matrix_v = (double **)malloc(dim * sizeof(double*));
	matrix_i = (int **)malloc(dim * sizeof(int*));
	KDarray->points = (SPPoint *) malloc(size * sizeof(*KDarray->points));
	if ((KDarray == NULL)||(matrix_v == NULL)||(matrix_i == NULL)||(KDarray->points == NULL)) { // Memory allocation error
		free(KDarray);
		free(matrix_v);
		free(matrix_i);
		free(KDarray->points);
		return NULL;
	}
	for (i=0;i<dim;i++) {
		matrix_v[i] = (double *)malloc(size * sizeof(double));
		matrix_i[i] = (int *)malloc(size * sizeof(int));
		if ((matrix_v[i] == NULL)||(matrix_i[i] == NULL)) { // Memory allocation error
			free(KDarray);
			free(matrix_v);
			free(matrix_i);
			free(KDarray->points);
			return NULL;
		}
	}
	// Function body
	KDarray->dim = dim;
	KDarray->size = size;
	for (i=0;i<size;i++) {
		KDarray->points[i] = spPointCopy(arr[i]); // The example from FinalProject.pdf (page 10) will look like:
		for (j=0;j<dim;j++) { //										|   1 | 123 |   2 |   9 |   3 |
			matrix_v[j][i] = spPointGetAxisCoor(arr[i],j); //matrix_v = |   2 |  70 |   7 |  11 |   4 |
			matrix_i[j][i] = i; //										|   0 |   1 |   2 |   3 |   4 |
		} //												 matrix_i = |   0 |   1 |   2 |   3 |   4 |
	}
	for (i=0;i<dim;i++) { // Foreach dim
		qqsort(matrix_i[i],size,sizeof(matrix_i[i][0]),spKDArrayCompare,matrix_v[i]);
	}
	KDarray->matrix = matrix_i;
	// Free memory
    for(i=0;i<dim;i++) {
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
SPKDArrayPair spKDArraySplit(SPKDArray kdArr, int coor) {
	// Function variables
	int i,j; // Generic loop variable
	int pointerLeft, pointerRight; // The pointers for the matrix splitting
	int spltr, tmpIndex; // Variables that hold some repeated calculations
	int* supportSide; // Two arrays that support the matrix splitting
	int* supportSub; // Two arrays that support the matrix splitting
	SPPoint* pointsLeft;
	SPPoint* pointsRight;
	int** matrixLeft;
	int** matrixRight;
	SPKDArrayPair res;
	// Allocate memory
	spltr = (kdArr->size)-(kdArr->size)/2; // Ex. 5/2=3
	supportSide = (int *) malloc(kdArr->size * sizeof(int));
	supportSub = (int *) malloc(kdArr->size * sizeof(int));
	pointsLeft = (SPPoint *) malloc(spltr * sizeof(*pointsLeft));
	pointsRight = (SPPoint *) malloc(((kdArr->size)-spltr) * sizeof(*pointsRight));
	matrixLeft = (int **)malloc(kdArr->dim * sizeof(int*));
	matrixRight = (int **)malloc(kdArr->dim * sizeof(int*));
	res = (SPKDArrayPair) malloc(sizeof(*res));
	res->left = (SPKDArray) malloc(sizeof(*res->left));
	res->right = (SPKDArray) malloc(sizeof(*res->right));
	if ((supportSide == NULL)||(supportSub == NULL)||(pointsLeft == NULL)||(pointsRight == NULL)||(matrixLeft == NULL)||(matrixRight == NULL)||(res == NULL)||(res->left == NULL)||(res->right == NULL)) { // Memory allocation error
		free(supportSide);
		free(supportSub);
		free(pointsLeft);
		free(pointsRight);
		free(matrixLeft);
		free(matrixRight);
		free(res);
		return NULL;
	}
	for (i=0;i<kdArr->dim;i++) {
		matrixLeft[i] = (int *)malloc(spltr * sizeof(int));
		matrixRight[i] = (int *)malloc(((kdArr->size)-spltr) * sizeof(int));
		if ((matrixLeft[i] == NULL)||(matrixRight[i] == NULL)) { // Memory allocation error
			free(supportSide);
			free(supportSub);
			free(pointsLeft);
			free(pointsRight);
			free(matrixLeft);
			free(matrixRight);
			free(res);
			return NULL;
		}
	}
	// Function body
	for (i=0;i<kdArr->size;i++) {
		tmpIndex = kdArr->matrix[coor][i];
		if (i < spltr) { // This point go to the left
			supportSide[tmpIndex] = 0;
			supportSub[tmpIndex] = i-tmpIndex;
			pointsLeft[i] = kdArr->points[tmpIndex];
		} else { // This point go to the right
			supportSide[tmpIndex] = 1;
			supportSub[tmpIndex] = i-spltr-tmpIndex;
			pointsRight[i-spltr] = kdArr->points[tmpIndex];
		}
	}
	// The example from FinalProject.pdf (page 10) will look like:
	// supportSide = [0,1,0,1,0]
	// supportSub = [0,0,-1,-3,-2]
	// pointsLeft = [a,c,e]
	// pointsRight = [d,b]
	// So far the complexity is O( MAX(d,n) )
	for (i=0;i<kdArr->dim;i++) {
		pointerLeft = 0;
		pointerRight = 0;
		for (j=0;j<kdArr->size;j++) {
			tmpIndex = kdArr->matrix[i][j];
			if (supportSide[tmpIndex] == 0) { // This point go to the left
				matrixLeft[i][pointerLeft] = tmpIndex+supportSub[tmpIndex];
				pointerLeft+=1;
			} else { // This point go to the right
				matrixRight[i][pointerRight] = tmpIndex+supportSub[tmpIndex];
				pointerRight+=1;
			}
		}
	}
	res->left->dim = kdArr->dim;
	res->left->size = spltr;
	res->left->points = pointsLeft;
	res->left->matrix = matrixLeft;
	res->right->dim = kdArr->dim;
	res->right->size = (kdArr->size)-spltr;
	res->right->points = pointsRight;
	res->right->matrix = matrixRight;
	// Free memory
    if (supportSide) { // A tiny chance for errors in some compilers
    	free(supportSide);
    	supportSide = NULL; // Preventing a "double-free"
    }
    if (supportSub) { // A tiny chance for errors in some compilers
    	free(supportSub);
    	supportSub = NULL; // Preventing a "double-free"
    }
	// Finish
	return res;
}

void spKDArrayDestroy(SPKDArray array) {
	int i;
	if (array != NULL) {
	    for(i=0;i<array->dim;i++) {
	        if (array->matrix[i]) { // A tiny chance for errors in some compilers
	        	free(array->matrix[i]);
	        }
	    }
	    if (array->matrix) { // A tiny chance for errors in some compilers
	    	free(array->matrix);
	    	array->matrix = NULL; // Preventing a "double-free"
	    }
	    if (array->points) { // A tiny chance for errors in some compilers
	    	free(array->points);
	    	array->points = NULL; // Preventing a "double-free"
	    }
		free(array);
	}
}

int spKDArrayGetDimension(SPKDArray array) {
	assert(array != NULL);
	return array->dim;
}

int spKDArrayGetSize(SPKDArray array) {
	assert(array != NULL);
	return array->size;
}

SPPoint* spKDArrayGetPoints(SPKDArray array) {
	assert(array != NULL);
	return array->points;
}

int** spKDArrayGetMatrix(SPKDArray array) {
	assert(array != NULL);
	return array->matrix;
}

SPKDArray spKDArrayPairGetLeft(SPKDArrayPair array) {
	assert(array != NULL);
	return array->left;
}

SPKDArray spKDArrayPairGetRight(SPKDArrayPair array) {
	assert(array != NULL);
	return array->right;
}
