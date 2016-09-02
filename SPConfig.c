#include "SPConfig.h"
#include "SPLogger.h"
#include <stdlib.h> // malloc, free, NULL
#include <stdio.h> // FILE, stdout, fopen, fclose, sprintf, printf, fflush, stdout
#include <stdbool.h> // bool, true, false
#include <string.h> // strcmp
#include <assert.h> // assert

#define STRING_LENGTH 1025 // 1024 + \0
#define BUFFER_SIZE 1024
#define NON_DEFAULT_CONFIG_PARAM 4
#define CONFIG_PARAM 14

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
void ConfigErrorMsg(const char* filename, int lineNumber,SP_CONFIG_MSG* msg,char* errorMsg){

	switch (*msg){
	case SP_CONFIG_MISSING_DIR:
		snprintf(errorMsg,STRING_LENGTH,"File: %s \nLine: %d \nMessage: Parameter spImagesDirectory is not set\n",filename, lineNumber);
		break;
	case SP_CONFIG_MISSING_PREFIX:
		snprintf(errorMsg,STRING_LENGTH,"File: %s \nLine: %d \nMessage: Parameter spImagesPrefix is not set\n",filename, lineNumber);
		break;
	case SP_CONFIG_MISSING_SUFFIX:
		snprintf(errorMsg,STRING_LENGTH,"File: %s \nLine: %d \nMessage: Parameter spImagesSuffix is not set\n",filename, lineNumber);
		break;
	case SP_CONFIG_MISSING_NUM_IMAGES:
		snprintf(errorMsg,STRING_LENGTH,"File: %s \nLine: %d \nMessage: Parameter spNumOfImages is not set\n",filename, lineNumber);
		break;
	case SP_CONFIG_CANNOT_OPEN_FILE:
		snprintf(errorMsg,STRING_LENGTH,"The configuration file %s couldn’t be open\n",filename);
		break;
	case SP_CONFIG_DEFAULT_CANNOT_OPEN_FILE:
		snprintf(errorMsg,STRING_LENGTH,"The default configuration file spcbir.config couldn’t be open\n");
		break;
	case SP_CONFIG_ALLOC_FAIL:
		snprintf(errorMsg,STRING_LENGTH,"An error occurred - allocation failure\n");
		break;
	case SP_CONFIG_INVALID_LINE:
		snprintf(errorMsg,STRING_LENGTH,"File: %s \nLine: %d \nMessage: Invalid configuration line\n",filename, lineNumber);
		break;
	case SP_CONFIG_INVALID_INTEGER:
		snprintf(errorMsg,STRING_LENGTH,"File: %s \nLine: %d \nMessage: Invalid value - constraint not met”\n",filename, lineNumber);
		break;
	case SP_CONFIG_INVALID_STRING:
		snprintf(errorMsg,STRING_LENGTH,"File: %s \nLine: %d \nMessage: Invalid value - constraint not met”\n",filename, lineNumber);
		break;
	case SP_CONFIG_INVALID_ARGUMENT:
		snprintf(errorMsg,STRING_LENGTH,"Invalid command line : use -c <config_filename>");
		break;
	case SP_CONFIG_INDEX_OUT_OF_RANGE:
		//TODO check when this error happens
		break;
	default:
		break;
	}

}
void SetDefaultConfigValues(SPConfig config){
	config->spPCADimension = 20;
	config->spPCAFilename = "pca.yml";
	config->spNumOfFeatures = 100;
	config->spExtractionMode = true;
	config->spNumOfSimilarImages = 1;
	config->spKDTreeSplitMethod = MAX_SPREAD;
	config->spKNN = 1;
	config->spMinimalGUI = false;
	config->spLoggerLevel = 3;
	config->spLoggerFilename = "stdout";
}
bool setConfigParameters(const SPConfig config,const char* variableName,const char* value,bool* nonDefaultParam, SP_CONFIG_MSG* msg ){

	if (strcmp(variableName,"\0") == 0){
		return true;
	}
    if (strcmp(variableName,"spImagesDirectory") == 0){
    	config->spImagesDirectory = (char*) malloc(strlen(value));
    	strcpy(config->spImagesDirectory, value);
    	*nonDefaultParam = true;
    } else
    if (strcmp(variableName,"spImagesPrefix") == 0){
    	config->spImagesPrefix = (char*) malloc(strlen(value));
    	strcpy(config->spImagesPrefix, value);
    	*(nonDefaultParam+1) = true;
    } else
    if (strcmp(variableName,"spImagesSuffix") == 0){
    	if((strcmp(value,".jpg") || strcmp(value,".png") || strcmp(value,".bmp") || strcmp(value,".gif")) != false){
        	config->spImagesSuffix = (char*) malloc(strlen(value));
        	strcpy(config->spImagesSuffix, value);
        	*(nonDefaultParam+2) = true;
    	} else {
    		*msg = SP_CONFIG_INVALID_STRING;
    		return false;
    	}
    } else
    if (strcmp(variableName,"spNumOfImages") == 0){
    	if (atoi(value)> 0){
        	config->spNumOfImages = atoi(value);
        	*(nonDefaultParam+3) = true;
    	} else {
    		*msg = SP_CONFIG_INVALID_INTEGER;
    		return false;
    	}
    } else
    if (strcmp(variableName,"spPCADimension") == 0){
    	if (atoi(value) > 28 || atoi(value) < 10){
    		*msg = SP_CONFIG_INVALID_INTEGER;
    		return false;
    	} else {
        	config->spPCADimension = atoi(value);
    	}
    } else
    if (strcmp(variableName,"spPCAFilename") == 0){
    	config->spPCAFilename = (char*) malloc(strlen(value));
    	strcpy(config->spPCAFilename, value);
    } else
    if (strcmp(variableName,"spNumOfFeatures") == 0){
    	if (atoi(value)> 0){
        	config->spNumOfFeatures = atoi(value);
    	} else {
    		*msg = SP_CONFIG_INVALID_INTEGER;
    		return false;
    	}
    } else
    if (strcmp(variableName,"spExtractionMode") == 0){
    	if (strcmp(value,"true") == 0){
    		config->spExtractionMode = true;
    	}else if(strcmp(value,"false") == 0){
    		config->spExtractionMode = false;
    	}else{
    		*msg = SP_CONFIG_INVALID_STRING;
    		return false;
    	}
    } else
    if (strcmp(variableName,"spNumOfSimilarImages") == 0){
    	if (atoi(value)> 0){
        	config->spNumOfSimilarImages = atoi(value);
    	} else {
    		*msg = SP_CONFIG_INVALID_INTEGER;
    		return false;
    	}
    } else
    if (strcmp(variableName,"spKDTreeSplitMethod") == 0){
        if (strcmp(variableName,"RANDOM") == 0)
    		config->spKDTreeSplitMethod = RANDOM;
        else if (strcmp(variableName,"MAX_SPREAD") == 0)
    		config->spKDTreeSplitMethod = MAX_SPREAD;
        else if (strcmp(variableName,"INCREMENTAL") == 0)
    		config->spKDTreeSplitMethod = INCREMENTAL;
        else
    		*msg = SP_CONFIG_INVALID_STRING;
    		return false;
    } else
    if (strcmp(variableName,"spKNN") == 0){
    	if (atoi(value)> 0){
        	config->spKNN = atoi(value);
    	} else {
    		*msg = SP_CONFIG_INVALID_INTEGER;
    		return false;
    	}
    } else
    if (strcmp(variableName,"spMinimalGUI") == 0){
    	if (strcmp(value,"true") == 0){
    		config->spMinimalGUI = true;
    	}else if(strcmp(value,"false") == 0){
    		config->spMinimalGUI = false;
    	}else{
    		*msg = SP_CONFIG_INVALID_STRING;
    		return false;
    	}
    } else
    if (strcmp(variableName,"spLoggerLevel") == 0){
    	if (atoi(value) > 4 || atoi(value) < 1){
    		*msg = SP_CONFIG_INVALID_INTEGER;
    		return false;
    	} else {
        	config->spLoggerLevel = atoi(value);
    	}
    } else
    if (strcmp(variableName,"spLoggerFilename") == 0){
    	config->spLoggerFilename = (char*) malloc(strlen(value));
    	strcpy(config->spLoggerFilename, value);
    } else {
		*msg = SP_CONFIG_INVALID_LINE;
    	return false;
	}
	return true;
}
int skipTabsAndWhitespace(const char* line, int lineC){
	while (line[lineC] == ' ' || line[lineC] == '\t'){ // skip tabs and whitespaces
		lineC++;
	}
	return lineC;
}
bool ParseLine(char* line,char* variableName,char* value, SP_CONFIG_MSG* msg){

	int lineC = 0, varC = 0, valC = 0;

	lineC = skipTabsAndWhitespace(line, lineC);
	if (line[lineC] == '#' || line[lineC] == '\n' ){
		*variableName = '\0';
		*value = '\0';
		return true;
	}
	if (line[lineC] == '='){
		*msg = SP_CONFIG_INVALID_LINE;
		return false;
	}
	while ((line[lineC] != ' ') && (line[lineC] != '\t') && (line[lineC] != '=')){ // copy variable Name
		variableName[varC] = line[lineC];
		lineC++;
		varC++;
	}
	variableName[varC] = '\0';
	lineC = skipTabsAndWhitespace(line, lineC);
	if (line[lineC] != '='){
		*msg = SP_CONFIG_INVALID_LINE;
		return false;
	}
	lineC++;
	lineC = skipTabsAndWhitespace(line, lineC);
	while (line[lineC] != ' ' && line[lineC] != '\t' && line[lineC] != '\n'){ // copy variable value
		value[valC] = line[lineC];
		lineC++;
		valC++;
	}
	value[valC] = '\0';
	lineC = skipTabsAndWhitespace(line, lineC);
	if (line[lineC] != '\n'){
		*msg = SP_CONFIG_INVALID_LINE;
		return false;
	}
	return true;
}
void FreeParseConfig(char* buffer,char* line,char* value,char* variableName){
	if (variableName)
		free(variableName);
	if (value)
		free(value);
	if(buffer)
		free(buffer);
	if(line)
		free(line);
}
bool ParseConfig(FILE* configFile,const SPConfig config, SP_CONFIG_MSG* msg, int* lineNumber ){
	char* buffer, *line, *variableName, *value;
	int numOfChar = 0, i = 0, p = 0;
	bool nonDefaultParam[NON_DEFAULT_CONFIG_PARAM] = {0}; // init the array to false
	bool parseSuccess;

	variableName = (char*) malloc(STRING_LENGTH);
	value = (char*) malloc(STRING_LENGTH);
	buffer = (char*) calloc(BUFFER_SIZE,sizeof(char));
	line = (char*) calloc(STRING_LENGTH,sizeof(char));
	if (variableName == NULL || value == NULL || buffer == NULL || line == NULL){
		*msg = SP_CONFIG_ALLOC_FAIL;
		FreeParseConfig(buffer,line,value,variableName);
		return false;
	}
	while ((numOfChar = fread(buffer,sizeof(char),BUFFER_SIZE,configFile)) > 0){
		for (i=0;i<numOfChar;i++){
			*(line+p) = *(buffer+i);
			p++;
			if (*(buffer+i)=='\n'){
				parseSuccess = ParseLine(line,variableName,value, msg);
				if (parseSuccess == false){
					FreeParseConfig(buffer,line,value,variableName);
					return false;
				}
				parseSuccess = setConfigParameters(config,variableName,value,nonDefaultParam, msg);
				if (parseSuccess == false){
					FreeParseConfig(buffer,line,value,variableName);
					return false;
				}
				(*lineNumber)++;
				p = 0;
			}
		}

	}
	for (i=0;i<NON_DEFAULT_CONFIG_PARAM;i++){
		if (nonDefaultParam[i] == false){
			switch (i){
			case 0:
				*msg = SP_CONFIG_MISSING_DIR;
				break;
			case 1:
				*msg = SP_CONFIG_MISSING_PREFIX;
				break;
			case 2:
				*msg = SP_CONFIG_MISSING_SUFFIX;
				break;
			case 3:
				*msg = SP_CONFIG_MISSING_NUM_IMAGES;
				break;
			}
			FreeParseConfig(buffer,line,value,variableName);
			return false;
		}
	}
	FreeParseConfig(buffer,line,value,variableName);
	return true;
}
SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){
	// Function variables
	SPConfig config;
	FILE* configFile;
	bool success;
	int lineNumber = 1;
	char errorMsg[STRING_LENGTH];
	if (filename == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		ConfigErrorMsg(filename, lineNumber,msg,errorMsg);
		printf(errorMsg);
		fflush(stdout);
		return NULL;
	}
	configFile = fopen(filename,"r");
	if (configFile == NULL){
		if (strcmp(filename,"spcbir.config") == 0)
			*msg = SP_CONFIG_DEFAULT_CANNOT_OPEN_FILE;
		else
			*msg = SP_CONFIG_CANNOT_OPEN_FILE;
		ConfigErrorMsg(filename, lineNumber,msg,errorMsg);
		printf(errorMsg);
		fflush(stdout);
		fclose(configFile);
		return NULL;
	}
	config = (SPConfig) malloc(sizeof(struct sp_config_t));
	if (config == NULL){
		*msg = SP_CONFIG_ALLOC_FAIL;
		ConfigErrorMsg(filename, lineNumber,msg,errorMsg);
		printf(errorMsg);
		fflush(stdout);
		fclose(configFile);
		return NULL;
	}
	SetDefaultConfigValues(config);
	success = ParseConfig(configFile, config, msg, &lineNumber);
	if (success == false){
		spConfigDestroy(config);
		ConfigErrorMsg(filename, lineNumber,msg,errorMsg);
		printf(errorMsg);
		fflush(stdout);
		fclose(configFile);
		return NULL;
	}
	*msg = SP_CONFIG_SUCCESS;
	fclose(configFile);
	return config;
}
bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg != NULL);
	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	else{
		*msg = SP_CONFIG_SUCCESS;
		return config->spExtractionMode;
	}
}


bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg != NULL);
	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	}
	else{
		*msg = SP_CONFIG_SUCCESS;
		return config->spMinimalGUI;
	}
}


int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg != NULL);
	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
	}
	else{
		*msg = SP_CONFIG_SUCCESS;
	}
	return config->spNumOfImages;
}


int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg != NULL);
	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
	}
	else{
		*msg = SP_CONFIG_SUCCESS;
	}
	return config->spNumOfFeatures;
}


int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg != NULL);
	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
	}
	else{
		*msg = SP_CONFIG_SUCCESS;
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
	snprintf(imagePath,STRING_LENGTH,"%s%s%d%s",config->spImagesDirectory,config->spImagesPrefix,
			index,config->spImagesSuffix);
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetFeatsPath(char* imagePath, const SPConfig config,int index){

	if (imagePath == NULL || config == NULL ){
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	if (index >= config->spNumOfImages){
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	}
	snprintf(imagePath,STRING_LENGTH,"%s%s%d%s",config->spImagesDirectory,config->spImagesPrefix,
			index,".feats");
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config){
	if (pcaPath == NULL || config == NULL ){
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	snprintf(pcaPath,STRING_LENGTH,"%s%s",config->spImagesDirectory,config->spPCAFilename);
	return SP_CONFIG_SUCCESS;
}

void spConfigDestroy(SPConfig config){
	if(config != NULL){
		free(config->spImagesDirectory);
		free(config->spImagesPrefix);
		free(config->spImagesSuffix);
		free(config->spPCAFilename);
		free(config->spLoggerFilename);
		free(config);
	}
}
