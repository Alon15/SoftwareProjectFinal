#include <stdlib.h> // malloc, free, NULL
#include <assert.h> // assert
#include <float.h> // DBL_MIN
#include "SPKDArray.h"
#include "SPPoint.h"
#include "sort_r.h"
#include "SPLogger.h"
#include "defines.h"

// Array datatype
struct kd_array_t {
	int dim; // The dimension of each point (d)
	int size; // Points array size (n)
	double* minSpread; // Array that contain the minimum value in each dimension
	double* maxSpread; // Array that contain the maximum value in each dimension
	SPPoint* points; // Array of points
	int** matrix; // Matrix for splitting purposes
};

struct kd_array_pair_t {
	SPKDArray left;
	SPKDArray right;
};

//
int spKDArrayCompare(const void *aIn, const void *bIn, const void *thunkIn) {
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

void specificMemoryFree(int* v0, int* v1, double* v2, double* v3, int** v4, int** v5, double** v6, double** v7, SPKDArray v8, SPKDArrayPair v9, SPPoint* v10, SPPoint* v11) {
	if (v0) { // A tiny chance for errors in some compilers
		free(v0);
		v0 = NULL; // Preventing a "double-free"
	}
	if (v1) { // A tiny chance for errors in some compilers
		free(v1);
		v1 = NULL; // Preventing a "double-free"
	}
	if (v2) { // A tiny chance for errors in some compilers
		free(v2);
		v2 = NULL; // Preventing a "double-free"
	}
	if (v3) { // A tiny chance for errors in some compilers
		free(v3);
		v3 = NULL; // Preventing a "double-free"
	}
	if (v4) { // A tiny chance for errors in some compilers
		free(v4);
		v4 = NULL; // Preventing a "double-free"
	}
	if (v5) { // A tiny chance for errors in some compilers
		free(v5);
		v5 = NULL; // Preventing a "double-free"
	}
	if (v6) { // A tiny chance for errors in some compilers
		free(v6);
		v6 = NULL; // Preventing a "double-free"
	}
	if (v7) { // A tiny chance for errors in some compilers
		free(v7);
		v7 = NULL; // Preventing a "double-free"
	}
	if (v8) { // A tiny chance for errors in some compilers
		free(v8);
		v8 = NULL; // Preventing a "double-free"
	}
	if (v9) { // A tiny chance for errors in some compilers
		free(v9);
		v9 = NULL; // Preventing a "double-free"
	}
	if (v10) { // A tiny chance for errors in some compilers
		free(v10);
		v10 = NULL; // Preventing a "double-free"
	}
	if (v11) { // A tiny chance for errors in some compilers
		free(v11);
		v11 = NULL; // Preventing a "double-free"
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
	KDarray->minSpread = (double *)malloc(dim * sizeof(double));
	KDarray->maxSpread = (double *)malloc(dim * sizeof(double));
	KDarray->points = (SPPoint *)malloc(size * sizeof(*KDarray->points));
	if ((KDarray == NULL)||(matrix_v == NULL)||(matrix_i == NULL)||(KDarray->minSpread == NULL)||(KDarray->maxSpread == NULL)||(KDarray->points == NULL)) { // Memory allocation error
		specificMemoryFree(NULL,NULL,KDarray->minSpread,KDarray->maxSpread,matrix_i,NULL,matrix_v,NULL,KDarray,NULL,KDarray->points,NULL);
		PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	for (i=0;i<dim;i++) {
		KDarray->minSpread[i] = DBL_MIN;
		KDarray->maxSpread[i] = DBL_MIN;
		matrix_v[i] = (double *)malloc(size * sizeof(double));
		matrix_i[i] = (int *)malloc(size * sizeof(int));
		if ((matrix_v[i] == NULL)||(matrix_i[i] == NULL)) { // Memory allocation error
			specificMemoryFree(NULL,NULL,KDarray->minSpread,KDarray->maxSpread,matrix_i,NULL,matrix_v,NULL,KDarray,NULL,KDarray->points,NULL);
			PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
			return NULL;
		}
	}
	// Function body
	KDarray->dim = dim;
	KDarray->size = size;
	for (i=0;i<size;i++) { //								 The example from FinalProject.pdf (page 10) will look like:
		KDarray->points[i] = spPointCopy(arr[i]); //					|   1 | 123 |   2 |   9 |   3 |
		for (j=0;j<dim;j++) { //							 matrix_v = |   2 |  70 |   7 |  11 |   4 |
			matrix_v[j][i] = spPointGetAxisCoor(arr[i],j); //			|   0 |   1 |   2 |   3 |   4 |
			matrix_i[j][i] = i; //							 matrix_i = |   0 |   1 |   2 |   3 |   4 |
			if ((KDarray->maxSpread[j] == DBL_MIN)||(KDarray->maxSpread[j] < matrix_v[j][i])) {
				KDarray->maxSpread[j] = matrix_v[j][i]; // Update the maximum spread for dim=j
			} else if ((KDarray->minSpread[j] == DBL_MIN)||(matrix_v[j][i] < KDarray->minSpread[j])) {
				KDarray->minSpread[j] = matrix_v[j][i]; // Update the minimum spread for dim=j
			}
		}
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

SPKDArrayPair spKDArraySplit(SPKDArray kdArr, int coor) {
	// Function variables
	double tmpLoopData;
	int i,j; // Generic loop variable
	int pointerLeft, pointerRight; // The pointers for the matrix splitting
	int spltr, tmpIndex; // Variables that hold some repeated calculations
	int* supportSide; // Two arrays that support the matrix splitting
	int* supportSub; // Two arrays that support the matrix splitting
	int** matrixLeft; // The matrix for the left SPKDArray
	int** matrixRight; // The matrix for the right SPKDArray
	double** minSpread; // Two dim array that hold the minimum spread for the left and for the right SPKDArray
	double** maxSpread; // Two dim array that hold the maximum spread for the left and for the right SPKDArray
	SPKDArrayPair res; // Variable for the results
	SPPoint* pointsLeft; // The points for the left SPKDArray
	SPPoint* pointsRight; // The points for the right SPKDArray
	// Allocate memory
	spltr = (kdArr->size)-(kdArr->size)/2; // Ex. 5/2=3
	supportSide = (int *)malloc(kdArr->size * sizeof(int));
	supportSub = (int *)malloc(kdArr->size * sizeof(int));
	matrixLeft = (int **)malloc(kdArr->dim * sizeof(int*));
	matrixRight = (int **)malloc(kdArr->dim * sizeof(int*));
	minSpread = (double **)malloc(2 * sizeof(double*)); // [0] is for the left & [1] for the right
	maxSpread = (double **)malloc(2 * sizeof(double*)); // [0] is for the left & [1] for the right
	res = (SPKDArrayPair) malloc(sizeof(*res));
	pointsLeft = (SPPoint *) malloc(spltr * sizeof(*pointsLeft));
	pointsRight = (SPPoint *) malloc(((kdArr->size)-spltr) * sizeof(*pointsRight));
	if ((supportSide==NULL)||(supportSub==NULL)||(matrixLeft==NULL)||(matrixRight==NULL)||(minSpread==NULL)||(maxSpread==NULL)||(res==NULL)||(pointsLeft==NULL)||(pointsRight==NULL)) { // Memory allocation error
		specificMemoryFree(supportSide,supportSub,NULL,NULL,matrixLeft,matrixRight,minSpread,maxSpread,NULL,res,pointsLeft,pointsRight);
		PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	minSpread[0] = (double *)malloc(kdArr->dim * sizeof(double));
	minSpread[1] = (double *)malloc(kdArr->dim * sizeof(double));
	maxSpread[0] = (double *)malloc(kdArr->dim * sizeof(double));
	maxSpread[1] = (double *)malloc(kdArr->dim * sizeof(double));
	res->left = (SPKDArray)malloc(sizeof(*res->left));
	res->right = (SPKDArray)malloc(sizeof(*res->right));
	if ((minSpread[0]==NULL)||(minSpread[1]==NULL)||(maxSpread[0]==NULL)||(maxSpread[1]==NULL)||(res->left==NULL)||(res->right==NULL)) { // Memory allocation error
		specificMemoryFree(supportSide,supportSub,NULL,NULL,matrixLeft,matrixRight,minSpread,maxSpread,NULL,res,pointsLeft,pointsRight);
		PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	for (i=0;i<kdArr->dim;i++) {
		minSpread[0][i] = DBL_MIN; // Reset the minimum spread of dim i in the left SPKDArray to be 'infinity'
		minSpread[1][i] = DBL_MIN; // Reset the minimum spread of dim i in the right SPKDArray to be 'infinity'
		maxSpread[0][i] = DBL_MIN; // Reset the maximum spread of dim i in the left SPKDArray to be 'infinity'
		maxSpread[1][i] = DBL_MIN; // Reset the maximum spread of dim i in the right SPKDArray to be 'infinity'
		matrixLeft[i] = (int *)malloc(spltr * sizeof(int));
		matrixRight[i] = (int *)malloc(((kdArr->size)-spltr) * sizeof(int));
		if ((matrixLeft[i] == NULL)||(matrixRight[i] == NULL)) { // Memory allocation error
			specificMemoryFree(supportSide,supportSub,NULL,NULL,matrixLeft,matrixRight,minSpread,maxSpread,NULL,res,pointsLeft,pointsRight);
			PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
			return NULL;
		}
	}
	// Function body
	for (i=0;i<kdArr->size;i++) {
		tmpIndex = kdArr->matrix[coor][i];
		if (i < spltr) { // This point go to the left
			supportSide[tmpIndex] = 0;
			supportSub[tmpIndex] = i-tmpIndex;
			pointsLeft[i] = spPointCopy(kdArr->points[tmpIndex]);
		} else { // This point go to the right
			supportSide[tmpIndex] = 1;
			supportSub[tmpIndex] = i-spltr-tmpIndex;
			pointsRight[i-spltr] = spPointCopy(kdArr->points[tmpIndex]);
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
				tmpLoopData = spPointGetAxisCoor(pointsLeft[matrixLeft[i][pointerLeft]],i);
				pointerLeft+=1;
			} else { // This point go to the right
				matrixRight[i][pointerRight] = tmpIndex+supportSub[tmpIndex];
				tmpLoopData = spPointGetAxisCoor(pointsRight[matrixRight[i][pointerRight]],i);
				pointerRight+=1;
			}
			if ((maxSpread[supportSide[tmpIndex]][i] == DBL_MIN)||(maxSpread[supportSide[tmpIndex]][i] < tmpLoopData)) {
				maxSpread[supportSide[tmpIndex]][i] = tmpLoopData; // Update the maximum spread for dim=i
			} else if ((minSpread[supportSide[tmpIndex]][i] == DBL_MIN)||(tmpLoopData < minSpread[supportSide[tmpIndex]][i])) {
				minSpread[supportSide[tmpIndex]][i] = tmpLoopData; // Update the minimum spread for dim=i
			}
		}
	}
	res->left->dim = kdArr->dim;
	res->left->size = spltr;
	res->left->minSpread = minSpread[0];
	res->left->maxSpread = maxSpread[0];
	res->left->points = pointsLeft;
	res->left->matrix = matrixLeft;
	res->right->dim = kdArr->dim;
	res->right->size = (kdArr->size)-spltr;
	res->right->minSpread = minSpread[1];
	res->right->maxSpread = maxSpread[1];
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
	if (minSpread) { // A tiny chance for errors in some compilers
		free(minSpread);
		minSpread = NULL; // Preventing a "double-free"
	}
	if (maxSpread) { // A tiny chance for errors in some compilers
		free(maxSpread);
		maxSpread = NULL; // Preventing a "double-free"
	}

	// Finish
	return res;
}

void spKDArrayPairDestroy(SPKDArrayPair arrayPair) {
	spKDArrayDestroy(spKDArrayPairGetLeft(arrayPair));
	spKDArrayDestroy(spKDArrayPairGetRight(arrayPair));
}

void spKDArrayDestroy(SPKDArray array) {
	int i;
	if (array != NULL) {
		for (i=0;i<array->dim;i++) {
			if (array->matrix[i]) { // A tiny chance for errors in some compilers
				free(array->matrix[i]);
			}
		}
		if (array->matrix) { // A tiny chance for errors in some compilers
			free(array->matrix);
			array->matrix = NULL; // Preventing a "double-free"
		}
		if (array->minSpread) { // A tiny chance for errors in some compilers
			free(array->minSpread); // TODO "No source available for ntdll!RtlInitUnicodeString()" error
			array->minSpread = NULL; // Preventing a "double-free"
		}
		if (array->maxSpread) { // A tiny chance for errors in some compilers
			free(array->maxSpread); // TODO "No source available for ntdll!RtlInitUnicodeString()" error
			array->maxSpread = NULL; // Preventing a "double-free"
		}
		if (array->points) { // A tiny chance for errors in some compilers // TODO
			for (i=0;i<array->size;i++) {
				spPointDestroy(array->points[i]);
				array->points[i] = NULL; // Preventing a "double-free"
			}
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

double* spKDArrayGetMinSpread(SPKDArray array) {
	assert(array != NULL);
	return array->minSpread;
}

double* spKDArrayGetMaxSpread(SPKDArray array) {
	assert(array != NULL);
	return array->maxSpread;
}

SPPoint* spKDArrayGetPoints(SPKDArray array) {
	assert(array != NULL);
	return array->points;
}

int** spKDArrayGetMatrix(SPKDArray array) {
	assert(array != NULL);
	return array->matrix;
}

SPKDArray spKDArrayPairGetLeft(SPKDArrayPair arrayPair) {
	assert(arrayPair != NULL);
	return arrayPair->left;
}

SPKDArray spKDArrayPairGetRight(SPKDArrayPair arrayPair) {
	assert(arrayPair != NULL);
	return arrayPair->right;
}
