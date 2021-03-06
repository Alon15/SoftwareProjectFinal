#include <stdlib.h> // malloc, free, NULL
#include <stdbool.h> // bool, true, false
#include <string.h> // strcmp, strcpy
#include <unistd.h> // F_OK, R_OK
#include "SPConfig.h"
#include "SPLogger.h"
#include "SPPoint.h"
#include "main_aux.h"
#include "defines.h"
#include "KDTreeNode.h"
#include "Extraction.h"

void getFileName(char* filename, int argc, char** argv) {
	if (argc > 3) {
		filename = NULL;
	} else if ((argc == 3)&&(argv[1][0] == '-')&&(argv[1][1] == 'c')) { // Special configuration file specified by "-c <config_filename>"
		strcpy(filename,argv[2]);
	} else { // Use default configuration file
		strcpy(filename,DEFAULT_CONFIG_FILE);
	}
}

SP_LOGGER_LEVEL parseLoggerLevel(int level) {
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
	if (config_msg != SP_CONFIG_SUCCESS) {
		PRINT(GET_LOGGER_FILENAME_FAIL_ERROR);
		return false;
	}
	loggerLevel = parseLoggerLevel(spConfigGetLoggerLevel(config, &config_msg)); // parse the logger level
	if (config_msg != SP_CONFIG_SUCCESS) {
		PRINT(LOGGER_LEVEL_INVALID_ERROR);
		return false;
	}
	if (strcmp(filename,"stdout") == 0) { // check if stdout was chosen to be output stream
		if (spLoggerCreate(NULL,loggerLevel) != SP_LOGGER_SUCCESS) { // create a logger that writes to stdout
			PRINT(DEFAULT_LOGGER_CREATE_FAIL_ERROR);
			return false;
		}
	} else {
		if (spLoggerCreate(filename,loggerLevel) != SP_LOGGER_SUCCESS) { // create a logger that writes to file
			PRINT(LOGGER_CREATE_FAIL_ERROR);
			return false;
		}
	}
	spLoggerPrintInfo(LOGGER_INIT_SUCCESS);
	return true;
}

void freeMainMemory(SPConfig config, SPPoint* featuresArray, int numOfFeats, bool logger,KDTreeNode kdTree) {
	int i;
	if (config) {
		spConfigDestroy(config);
	}
	if (logger) {
		spLoggerDestroy();
	}
	if (featuresArray) {
		for (i=0;i<numOfFeats;i++) {
			spPointDestroy(featuresArray[i]);
		}
		free (featuresArray);
		featuresArray = NULL;
	}
	if (kdTree) {
		spKDTreeDestroy(kdTree);
	}
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
void freeExtractAllFeaturesMemory(SPPoint** imageArray, int numOfImages, int* numOfFeatsArray, SPPoint* featuresArray, bool subArray) {
	int i;
	if (imageArray && numOfFeatsArray) {
		for (i=0;i<numOfImages;i++) {
			if (subArray) {
				FREE_FEATURES_ARRAY(imageArray[i],numOfFeatsArray[i]); // free the points in the sub-arrays
			} else {
				free(imageArray[i]); // free only the array
			}
		}
		free(imageArray);
		free(numOfFeatsArray);
		imageArray = NULL;
		numOfFeatsArray = NULL;
	}
	if (imageArray) { // numOfFeatsArray might be NULL, so only imageArray need to be freed
		free(imageArray);
	}
	if (featuresArray) {
		free(featuresArray);
	}
}

SPPoint* extractAllFeatures(SPConfig config, int* numOfFeats) {
	// Function variables
	int numOfImages, i, j, k;
	int* numOfFeatsArray = NULL; // array of number of feature (correspond to a feature array)
	SPPoint** imageArray = NULL; // array of feature arrays
	char featurePath[STRING_LENGTH];
	SPPoint* featuresArray = NULL;
	SP_CONFIG_MSG config_msg = SP_CONFIG_SUCCESS;
	numOfImages = spConfigGetNumOfImages(config,&config_msg);
	if (config_msg != SP_CONFIG_SUCCESS) {
		PRINT_ERROR_LOGGER(GET_NUM_OF_IMAGES_FAIL_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	// Memory allocation
	imageArray = (SPPoint**)malloc(sizeof(SPPoint*)*numOfImages);
	numOfFeatsArray = (int*)malloc(sizeof(int)*numOfImages);
	if (imageArray == NULL || numOfFeatsArray == NULL) {
		freeExtractAllFeaturesMemory(imageArray,0,numOfFeatsArray,NULL,true);
		PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	// Function code
	*numOfFeats = 0;
	for (i=0;i<numOfImages;i++) {
		config_msg = spConfigGetFeatsPath(featurePath,config,i);
		if (config_msg != SP_CONFIG_SUCCESS) {
			PRINT_ERROR_LOGGER(GET_FEATS_PATH_FAIL_ERROR,__FILE__,__func__,__LINE__);
			freeExtractAllFeaturesMemory(imageArray,i-1,numOfFeatsArray,NULL,true);
			return NULL;
		}
		imageArray[i] = ImportFeats(featurePath,&(numOfFeatsArray[i]));
		if (imageArray[i] == NULL) {
			freeExtractAllFeaturesMemory(imageArray,i-1,numOfFeatsArray,NULL,true);
			return NULL;
		}
		*numOfFeats += numOfFeatsArray[i];
	}
	featuresArray = (SPPoint*)malloc((*numOfFeats)*sizeof(SPPoint));
	if (featuresArray == NULL) {
		freeExtractAllFeaturesMemory(imageArray,numOfImages,numOfFeatsArray,featuresArray,0);
		PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	k = 0;
	for (i=0;i<numOfImages;i++) {
		for (j=0;j<numOfFeatsArray[i];j++) {
			featuresArray[k] = imageArray[i][j];
			k++;
		}
	}
	freeExtractAllFeaturesMemory(imageArray,numOfImages,numOfFeatsArray,NULL,false);
	return featuresArray;
}

bool fileCheck(const char* fileName ){
	if(access(fileName, F_OK )) {
		PRINT(FILE_NOT_EXISTS);
		return false;
	}
	if(access(fileName, R_OK )) {
		PRINT(FILE_CANT_READ);
		return false;
	}
	return true;
}
