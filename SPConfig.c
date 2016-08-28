#include "SPConfig.h"
#include "SPLogger.h"
#include <stdlib.h> // malloc, free, NULL
#include <stdio.h> // FILE, stdout, fopen, fclose, sprintf, printf, fflush, stdout
#include <stdbool.h> // bool, true, false
#include <assert.h> // assert

#define STRING_LENGTH 1025
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

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){
	// Function variables
	SPConfig config;
	FILE* configFile;
	bool success;

	configFile = fopen(filename,"r");
	if (configFile == NULL){
		return SP_CONFIG_CANNOT_OPEN_FILE;
	}
	config = (SPConfig) malloc(sizeof(struct sp_config_t));
	if (config == NULL){
		return SP_CONFIG_ALLOC_FAIL;
	}
	SetDefaultConfigValues(config);
	success = ParseConfig(configFile, config);
	if (success == false){
		//TODO terminate the program
		return NULL;
	}
	return SP_CONFIG_SUCCESS;
}

bool setConfigParameters(SPConfig config,char* variableName,char* value, int lineNumber,bool* nonDefaultParam ){

	switch(variableName){
    case 'spImagesDirectory' :
    	config->spImagesDirectory = (char*) malloc(strlen(value));
    	strcpy(config->spImagesDirectory, value);
    	*nonDefaultParam = true; //TODO test if it works
        break;
    case 'spImagesPrefix' :
    	config->spImagesPrefix = (char*) malloc(strlen(value));
    	strcpy(config->spImagesPrefix, value);
    	*(nonDefaultParam+1) = true;
        break;
    case 'spImagesSuffix' :
    	if(value == '.jpg' || value == '.png' || value == '.bmp' || value == '.gif'){
        	config->spImagesSuffix = (char*) malloc(strlen(value));
        	strcpy(config->spImagesSuffix, value);
        	*(nonDefaultParam+2) = true;
    	} else {
    		//TODO error, invalid suffix
    	}
        break;
    case 'spNumOfImages' :
    	if (atoi(value)> 0){
        	config->spNumOfImages = atoi(value);
        	*(nonDefaultParam+3) = true;
    	} else {
    		//TODO error, negative number or NAN
    	}
        break;
    case 'spPCADimension' :
    	if (atoi(value) > 28 || atoi(value) < 10){
    		//TODO error, out of range or NAN
    	} else {
        	config->spPCADimension = atoi(value);
    	}
        break;
    case 'spPCAFilename' :
    	config->spPCAFilename = (char*) malloc(strlen(value));
    	strcpy(config->spPCAFilename, value);
        break;
    case 'spNumOfFeatures' :
    	if (atoi(value)> 0){
        	config->spNumOfFeatures = atoi(value);
    	} else {
    		//TODO error, negative number or NAN
    	}
        break;
    case 'spExtractionMode' :
    	if (value == "true"){
    		config->spExtractionMode = true;
    	}else if(value == "false"){
    		config->spExtractionMode = false;
    	}else{
    		//TODO error invalid string
    	}
        break;
    case 'spNumOfSimilarImages' :
    	if (atoi(value)> 0){
        	config->spNumOfSimilarImages = atoi(value);
    	} else {
    		//TODO error, negative number or NAN
    	}
        break;
    case 'spKDTreeSplitMethod' :
    	switch(value){
    	case 'RANDOM':
    		config->spKDTreeSplitMethod = RANDOM;
    		break;
    	case 'MAX_SPREAD':
    		config->spKDTreeSplitMethod = MAX_SPREAD;
    		break;
    	case 'INCREMENTAL':
    		config->spKDTreeSplitMethod = INCREMENTAL;
    		break;
        default :
        	//TODO error
    	}
        break;
    case 'spKNN' :
    	if (atoi(value)> 0){
        	config->spKNN = atoi(value);
    	} else {
    		//TODO error, negative number or NAN
    	}
        break;
    case 'spMinimalGUI' :
    	if (value == "true"){
    		config->spMinimalGUI = true;
    	}else if(value == "false"){
    		config->spMinimalGUI = false;
    	}else{
    		//TODO error invalid string
    	}
        break;
    case 'spLoggerLevel' :
    	if (atoi(value) > 4 || atoi(value) < 1){
    		//TODO error, out of range or NAN
    	} else {
        	config->spLoggerLevel = atoi(value);
    	}
        break;
    case 'spLoggerFilename' :
    	config->spLoggerFilename = (char*) malloc(strlen(value));
    	strcpy(config->spLoggerFilename, value);
        break;
    default :
       	//TODO error, invalid variable name
    	return false;
	}
	return true;
}
bool ParseConfig(FILE* configFile, SPConfig config ){
	char* buffer, line, p, variableName, value;
	int numOfChar = 0, i = 0, lineNumber = 1;
	bool nonDefaultParam[NON_DEFAULT_CONFIG_PARAM] = {0}; // init the array to false
	bool parseSuccess;

	variableName = (char*) malloc(STRING_LENGTH);
	value = (char*) malloc(STRING_LENGTH);
	buffer = (char*) calloc(BUFFER_SIZE,sizeof(char));
	line = (char*) calloc(STRING_LENGTH,sizeof(char));
	if (variableName == NULL || value == NULL || buffer == NULL || line == NULL){
		//TODO free things
		return false
	}
	p = line;
	while ((numOfChar = fread(buffer,sizeof(char),BUFFER_SIZE,configFile)) > 0){

		for (i=0;i<numOfChar;i++){
			line[p] = buffer[i];
			p++;
			if (buffer[i]=="\n"){
				parseSuccess = Parseline(line,variableName,value,lineNumber);
				if (parseSuccess == false){
					//TODO terminate the program + free
					return false;
				}
				parseSuccess = setConfigParameters(config,variableName,value,lineNumber,nonDefaultParam);
				if (parseSuccess == false){
					//TODO terminate the program + free
					return false;
				}
				lineNumber++;
				p = line;
			}
		}

	}
	for (i=0;i<NON_DEFAULT_CONFIG_PARAM;i++){
		if (nonDefaultParam[i] == false){
			//TODO error, non default parameter was not set + free
			return false;
		}
	}
	free(buffer);
	free(line);
	free(value);
	free(variableName);
	return true;
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

bool ParseLine(char* line, char* variableName, char* value, int lineNumber){

	int lineC = 0, varC = 0, valC = 0;

	lineC = skipTabsAndWhitespace(line, lineC);
	if (line[lineC] == '#' || line[lineC] == '\n' ){
		*variableName = NULL;
		*value = NULL;
		return true;
	}
	if (line[lineC] == '='){
		//TODO invalid line, throw error
		return false;
	}
	while (line[lineC] != ' ' || line[lineC] != '\t' || line[lineC] != '='){ // copy variable Name
		variableName[varC] = line[lineC];
		lineC++;
		varC++;
	}
	variableName[varC] = '\n';
	lineC = skipTabsAndWhitespace(line, lineC);
	if (line[lineC] != '='){
		//TODO invalid line, throw error
		return false;
	}
	lineC++;
	lineC = skipTabsAndWhitespace(line, lineC);
	while (line[lineC] != ' ' || line[lineC] != '\t' || line[lineC] != '\n'){ // copy variable value
		value[valC] = line[lineC];
		lineC++;
		valC++;
	}
	value[valC] = '\n';
	lineC = skipTabsAndWhitespace(line, lineC);
	if (line[lineC] != '\n'){
		//TODO invalid line, throw error
		return false;
	}
	return true;
}

int skipTabsAndWhitespace(char* line, int lineC){
	while (line[lineC] == ' ' || line[lineC] == '\t'){ // skip tabs and whitespaces
		lineC++;
	}
	return lineC;
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
	snprintf(imagePath,STRING_LENGTH,"%s%s%d%s",config->spImagesDirectory,config->spImagesPrefix,
			index,config->spImagesSuffix);
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
