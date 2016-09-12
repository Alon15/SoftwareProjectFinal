#include "SPConfig.h"
#include "SPLogger.h"
#include "SPPoint.h"
#include "main_aux.h"
#include "defines.h"
#include "KDTreeNode.h"
#include "Extraction.h"
#include <stdlib.h> // malloc, free, NULL
#include <stdio.h> // FILE, stdout, fopen, fclose, sprintf, printf, fflush, stdout
#include <stdbool.h> // bool, true, false
#include <string.h> // strcmp
#include "SPKDArray.h" // TODO DEBUG DELME

void getFileName(char* filename, int argc, char** argv) {
	if (argc > 3) {
		filename = NULL;
	} else if ((argc == 3)&&(argv[1][0] == '-')&&(argv[1][1] == 'c')) { // Special configuration file specified by "-c <config_filename>"
		strcpy(filename,argv[2]);
	} else { // Use default configuration file
		strcpy(filename,DEFAULT_CONFIG_FILE);
	}
}
SP_LOGGER_LEVEL parseLoggerLevel(int level){
	switch(level) {
	case 1:
		return SP_LOGGER_ERROR_LEVEL;
	case 2:
		return SP_LOGGER_WARNING_ERROR_LEVEL;
	case 3:
		return SP_LOGGER_INFO_WARNING_ERROR_LEVEL;
	case 4:
		return SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL;
	}
	return SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL;
}
bool initLogger(SPConfig config){
	// Function variables
	SP_CONFIG_MSG config_msg = SP_CONFIG_SUCCESS;
	SP_LOGGER_LEVEL loggerLevel;
	char filename[STRING_LENGTH];
	// Function code
	config_msg = spConfigGetLoggerFileName(filename,config); // get the logger filename
	if (config_msg != SP_CONFIG_SUCCESS){
		PRINT(GET_LOGGER_FILENAME_FAIL_ERROR);
		return false;
	}
	loggerLevel = parseLoggerLevel(spConfigGetLoggerLevel(config, &config_msg)); // parse the logger level
	if (config_msg != SP_CONFIG_SUCCESS){
		PRINT(LOGGER_LEVEL_INVALID_ERROR);
		return false;
	}
	if (strcmp(filename,"stdout") == 0){ // check if stdout was chosen to be output stream
		if (spLoggerCreate(NULL,loggerLevel) != SP_LOGGER_SUCCESS){ // create a logger that writes to stdout
			PRINT(DEFAULT_LOGGER_CREATE_FAIL_ERROR);
			return false;
		}
	} else {
		if (spLoggerCreate(filename,loggerLevel) != SP_LOGGER_SUCCESS){ // create a logger that writes to file
			PRINT(LOGGER_CREATE_FAIL_ERROR);
			return false;
	}
	}
	spLoggerPrintInfo(LOGGER_INIT_SUCCESS);
	return true;
}

void freeMainMemory(SPConfig config,SPPoint* featuresArray,int numOfFeats, bool logger){
	int i;
	if (config)
		spConfigDestroy(config);
	if (logger)
		spLoggerDestroy();
	if (featuresArray){
		for (i=0;i<numOfFeats;i++){
			spPointDestroy(featuresArray[i]);
		}
		free (featuresArray);
		featuresArray = NULL;
	}
	//TODO free KDTree here
}

/*
 * Free the memory allocted in 'extractAllFeatures'
 *
 * Freeing featuresArray will also free the memory of all the points stored in the array.
 * Freeing imageArray will also free the memory of all the arrays stored in it, and all
 * the points stored in those arrays.
 * If a parameter is NULL, then it won't be freed .
 *
 */
void freeExtractAllFeaturesMemory(SPPoint** imageArray,int numOfImages,int* numOfFeatsArray,
		SPPoint* featuresArray,int totalNumOfFeatures){
	int i;
	if (imageArray && numOfFeatsArray){
		for (i=0;i<numOfImages;i++){
			FREE_FEATURES_ARRAY(imageArray[i],numOfFeatsArray[i]); // free the sub-arrays
		}
		free(imageArray);
		free(numOfFeatsArray);
		imageArray = NULL;
		numOfFeatsArray = NULL;
	}
	if (imageArray) // numOfFeatsArray might be NULL, so only imageArray need to be freed
		free(imageArray);
	if (featuresArray)
		FREE_FEATURES_ARRAY(featuresArray,totalNumOfFeatures);
}
/*
 * Extract all the features from the ".feats" files of all the images in the directory, and store
 * the features extracted in 'featuresArray', also store the number of features extracted in 'numOfFeats'
 *
 *  @param config - the configuration structure
 *  @param featuresArray - a pointer to the array that will store the eatures
 *  @param numOfFeats - a pointer to a integer that will store the number of features extracted
 *
 *  @return True if the features successfully extracted
 *  		False if extraction failed (an error message will be displayed)
 */
bool extractAllFeatures(SPConfig config,SPPoint* featuresArray,int* numOfFeats){
	// Function variables
	int numOfImages, i, j, k;
	int* numOfFeatsArray = NULL; // array of number of feature (correspond to a feature array)
	SPPoint** imageArray = NULL; // array of feature arrays
	char featurePath[STRING_LENGTH];
	SP_CONFIG_MSG config_msg = SP_CONFIG_SUCCESS;
	numOfImages = spConfigGetNumOfImages(config,&config_msg);
	if (config_msg != SP_CONFIG_SUCCESS) {
		PRINT_ERROR_LOGGER(GET_NUM_OF_IMAGES_FAIL_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	// Memory allocation
	imageArray = (SPPoint**) malloc(sizeof(SPPoint*)*numOfImages);
	numOfFeatsArray = (int*) malloc(sizeof(int)*numOfImages);
	if (imageArray == NULL || numOfFeatsArray == NULL){
		freeExtractAllFeaturesMemory(imageArray,0,numOfFeatsArray,NULL,0);
		PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	// Function code
	*numOfFeats = 0;
	for (i=0;i<numOfImages;i++){
		config_msg = spConfigGetFeatsPath(featurePath,config,i);
		if (config_msg != SP_CONFIG_SUCCESS) {
			PRINT_ERROR_LOGGER(GET_FEATS_PATH_FAIL_ERROR,__FILE__,__func__,__LINE__);
			freeExtractAllFeaturesMemory(imageArray,i-1,numOfFeatsArray,NULL,0);
			return false;
		}
		imageArray[i] = ImportFeats(featurePath,&(numOfFeatsArray[i]));
		*numOfFeats += numOfFeatsArray[i];
	}
	featuresArray = (SPPoint*) malloc((*numOfFeats)*sizeof(SPPoint*));
	if (featuresArray == NULL){
		freeExtractAllFeaturesMemory(imageArray,numOfImages,numOfFeatsArray,featuresArray,0);
		PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	k = 0;
	for (i=0;i<numOfImages;i++){
		for (j=0;j<numOfFeatsArray[i];j++){
			featuresArray[k] = imageArray[i][j];
			k++;
		}
	}
	freeExtractAllFeaturesMemory(imageArray,numOfImages,numOfFeatsArray,NULL,0);
	return true;
}

bool initKDTree(SPConfig config, KDTreeNode kdTree){
	SPPoint* featuresArray;
	int numOfFeats;
	if (!extractAllFeatures(config,featuresArray,&numOfFeats)){
		return false;
	}
	kdTree = NULL;
	//TODO init the KDTree here
	return true;
}

void tmpFunc1() { // TODO DEBUG DELME
	double d0[] = {1,2};
	double d1[] = {123,70};
	double d2[] = {2,7};
	double d3[] = {9,11};
	double d4[] = {3,4};
	SPPoint p0 = spPointCreate(d0,2,0);
	SPPoint p1 = spPointCreate(d1,2,1);
	SPPoint p2 = spPointCreate(d2,2,2);
	SPPoint p3 = spPointCreate(d3,2,3);
	SPPoint p4 = spPointCreate(d4,2,4);
	SPPoint pR0[] = {p0,p1,p2,p3,p4};
	SPPoint pR1[] = {p2,p2,p1,p1,p3};
	SPPoint pR2[] = {p2,p3,p3,p4,p4};
	SPPoint pR3[] = {p2,p3,p2,p2,p3};
	SPKDArray k0 = Init(pR0,5);
	SPKDArray k1 = Init(pR1,5);
	SPKDArray k2 = Init(pR2,5);
	SPKDArray k3 = Init(pR3,5);
	//int dim = k->dim;
	printf("OK\n");
	printf("A: %d\n",spPointGetIndex(pR0[0]));
	printf("B: %d\n",spPointGetDimension(pR0[0]));
	fflush(stdout);
	int d = spKDArrayGetDimension(k0);
	int s = spKDArrayGetSize(k0);
	printf("C: %d\n",d);
	printf("D: %d\n",s);
	fflush(stdout);
	int** mtrx = spKDArrayGetMatrix(k0);
	for (int j=0;j<d;j++) {
		for (int i=0;i<s;i++) {
			printf("| %d ",mtrx[j][i]);
		}
		printf("|\n");
	}
	fflush(stdout);
	SPKDArrayPair kP[] = {Split(k0,0),Split(k1,0),Split(k2,0),Split(k3,0)};
	printf("tt1\n");
	fflush(stdout);
	for (int k=0;k<4;k++) {
		mtrx = spKDArrayGetMatrix(spKDArrayPairGetLeft(kP[k]));
		printf("Test %d Left\n",k);
		fflush(stdout);
		for (int j=0;j<d;j++) {
			for (int i=0;i<s;i++) {
				printf("| %d ",mtrx[j][i]);
			}
			printf("|\n");
		}
		printf("Test %d Right\n",k);
		fflush(stdout);
		mtrx = spKDArrayGetMatrix(spKDArrayPairGetRight(kP[k]));
		for (int j=0;j<d;j++) {
			for (int i=0;i<s;i++) {
				printf("| %d ",mtrx[j][i]);
			}
			printf("|\n");
		}
		fflush(stdout);
	}
}
