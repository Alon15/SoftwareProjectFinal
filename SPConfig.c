#include "SPConfig.h"
#include "SPLogger.h"
#include <stdlib.h> // malloc, free, NULL
#include <stdio.h> // FILE, stdout, fopen, fclose, sprintf, printf, fflush, stdout
#include <stdbool.h> // bool, true, false
#include <assert.h> // assert

#define STRING_LENGTH 1024

struct sp_config_t {
	// Input parameters
	char* spImagesDirectory;
	char* spImagesPrefix;
	char* spImagesSuffix;
	int spNumOfImages;
	// Image processing parameters
	int spPCADimension; // default value = 20
	char* spPCAFilename; // default value = pca.yml
	int spNumOfFeatures; // default value = 100
	bool spExtractionMode; // default value = true
	int spNumOfSimilarImages; // default value= 1
	// KDTree parameters
	SP_SPLIT_METHOD spKDTreeSplitMethod; // default value= MAX_SPREAD
	int spKNN; // default value= 1
	// Output parameters
	bool spMinimalGUI; // default value = false
	int spLoggerLevel; // default value= 3
	char* spLoggerFilename; // default value= stdout

};

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){
	// Function variables
	SPConfig config;
	FILE* configFile;

	config = (SPConfig) malloc(sizeof(struct sp_config_t));

	//TODO finish it
}

bool ParseConfig(FILE* configFile, SPConfig config ){
	//TODO finish it

}

bool ParseLine(const char* line, const char* variableName, const char* value ){
	//TODO finish it
	// return true if the line is valid, false otherwise
}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg != NULL);
	if (config == NULL){
		msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	else{
		msg = SP_CONFIG_SUCCESS;
		return config->spExtractionMode;
	}
}


bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg != NULL);
	if (config == NULL){
		msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	else{
		msg = SP_CONFIG_SUCCESS;
		return config->spMinimalGUI;
	}
}


int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg != NULL);
	if (config == NULL){
		msg = SP_CONFIG_INVALID_ARGUMENT;
	}
	else{
		msg = SP_CONFIG_SUCCESS;
	}
	return config->spNumOfImages;
}


int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg != NULL);
	if (config == NULL){
		msg = SP_CONFIG_INVALID_ARGUMENT;
	}
	else{
		msg = SP_CONFIG_SUCCESS;
	}
	return config->spNumOfFeatures;
}


int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg != NULL);
	if (config == NULL){
		msg = SP_CONFIG_INVALID_ARGUMENT;
	}
	else{
		msg = SP_CONFIG_SUCCESS;
	}
	return config->spPCADimension;
}

SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,int index){

	if (imagePath == NULL || config == NULL ){
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	if (index >= config->spNumOfImages){
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	}
	snprintf(imagePath,STRING_LENGTH+1,"%s%s%d%s",config->spImagesDirectory,config->spImagesPrefix,
			index,config->spImagesSuffix);
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config){
	if (pcaPath == NULL || config == NULL ){
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	snprintf(pcaPath,STRING_LENGTH+1,"%s%s",config->spImagesDirectory,config->spPCAFilename);
	return SP_CONFIG_SUCCESS;
}

void spConfigDestroy(SPConfig config){
	if(config != NULL){
		//TODO free memory
	}
}
