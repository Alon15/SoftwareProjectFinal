#include <stdlib.h> // malloc, free, NULL
#include <stdio.h> // FILE, stdout, fopen, fclose, sprintf, printf, fflush, stdout
#include <stdbool.h> // bool, true, false
#include <string.h> // strcmp
#include <assert.h> // assert
#include "SPConfig.h"
#include "SPLogger.h"
#include "defines.h"

struct sp_config_t {
	// Input parameters
	char* spImagesDirectory;
	char* spImagesPrefix;
	char* spImagesSuffix;
	int spNumOfImages;
	// Image processing parameters
	int spPCADimension;						// default value = 20
	char* spPCAFilename;					// default value = pca.yml
	int spNumOfFeatures;					// default value = 100
	bool spExtractionMode;					// default value = true
	int spNumOfSimilarImages;				// default value= 1
	// KDTree parameters
	SP_SPLIT_METHOD spKDTreeSplitMethod;	// default value= MAX_SPREAD
	int spKNN;								// default value= 1
	// Output parameters
	bool spMinimalGUI;						// default value = false
	int spLoggerLevel;						// default value= 3
	char* spLoggerFilename;					// default value= stdout
};

/*
 * The function store in errorMsg a string that match the type of error represented by msg.
 *
 * @param filename - The name of the config file.
 * @param lineNumber - The line in the config file that caused the error.
 * @param msg - Pointer to the massage that represent the error that occurred.
 * @param errorMsg - String of the formatted error message.
 */
void ConfigErrorMsg(const char* filename, int lineNumber, SP_CONFIG_MSG* msg, char* errorMsg) {
	switch (*msg) {
		case SP_CONFIG_MISSING_DIR: // spImagesDirectory is missing
			CONFIG_NON_DEFAULT_PARAMETER_MISSING(errorMsg,filename,lineNumber,"spImagesDirectory");
			break;
		case SP_CONFIG_MISSING_PREFIX: // spImagesPrefix is missing
			CONFIG_NON_DEFAULT_PARAMETER_MISSING(errorMsg,filename,lineNumber,"spImagesPrefix");
			break;
		case SP_CONFIG_MISSING_SUFFIX: // spImagesSuffix is missing 
			CONFIG_NON_DEFAULT_PARAMETER_MISSING(errorMsg,filename,lineNumber,"spImagesSuffix");
			break;
		case SP_CONFIG_MISSING_NUM_IMAGES: // spNumOfImages is missing
			CONFIG_NON_DEFAULT_PARAMETER_MISSING(errorMsg,filename,lineNumber,"spNumOfImages");
			break;
		case SP_CONFIG_CANNOT_OPEN_FILE: // The configuration file given by filename cannot be open
			CONFIG_CANT_OPEN_FILE(errorMsg,filename);
			break;
		case SP_CONFIG_DEFAULT_CANNOT_OPEN_FILE: // The default configuration file cannot be open
			CONFIG_DEFAULT_CANT_OPEN_FILE(errorMsg);
			break;
		case SP_CONFIG_ALLOC_FAIL: // An allocation failure occurred
			CONFIG_ALLOC_FAIL(errorMsg);
			break;
		case SP_CONFIG_INVALID_LINE: // A line in the config file has invalid structure
			CONFIG_INVALID_LINE(errorMsg,filename,lineNumber);
			break;
		case SP_CONFIG_INVALID_INTEGER: // A line in the config file contains invalid integer
			CONFIG_INVALID_VALUE(errorMsg,filename,lineNumber);
			break;
		case SP_CONFIG_INVALID_STRING: // A line in the config file contains invalid string
			CONFIG_INVALID_VALUE(errorMsg,filename,lineNumber);
			break;
		case SP_CONFIG_INVALID_ARGUMENT: // filename == NULL
			CONFIG_INVALID_ARGUMENT(errorMsg);
			break;
		default:
			break;
	}
}
/*
 * The function set the default values to the config.
 *
 * @param config - The configuration structure.
 */
void SetDefaultConfigValues(SPConfig config) {
	config->spPCADimension = 20;
	config->spNumOfFeatures = 100;
	config->spExtractionMode = true;
	config->spNumOfSimilarImages = 1;
	config->spKDTreeSplitMethod = MAX_SPREAD;
	config->spKNN = 1;
	config->spMinimalGUI = false;
	config->spLoggerLevel = 3;
	config->spLoggerFilename = NULL;
	config->spPCAFilename = NULL;
}

/*
 * The function set the default values to the string parameters in the config.
 *
 * @param config - The configuration structure.
 */
bool SetDefaultConfigString(SPConfig config, SP_CONFIG_MSG* msg){
	if (config->spPCAFilename == NULL){
		config->spPCAFilename = (char*) malloc(strlen("pca.yml")+1);
		if (config->spPCAFilename == NULL){
			*msg = SP_CONFIG_ALLOC_FAIL;
			return false;
		}
		strcpy(config->spPCAFilename, "pca.yml");
	}
	if (config->spLoggerFilename == NULL){
		config->spLoggerFilename = (char*) malloc(strlen("stdout")+1);
		if (config->spLoggerFilename == NULL){
			*msg = SP_CONFIG_ALLOC_FAIL;
			return false;
		}
		strcpy(config->spLoggerFilename, "stdout");
	}
	return true;
}
/*
 * The function sets a given value to his field in the configuration structure.
 *
 * If variableName is a valid field of the configuration structure
 * and value is valid value for this field
 * Then the function set the value to this field and return true.
 * Else set msg to the relevant error and return false.
 *
 * If variableName is one of the non Default parameters, then the function set the matching cell in "nonDefaultParam"
 * array to be true.
 *
 * If variableName is an empty string the function do nothing and return true.
 *
 * @param config - The configuration structure.
 * @param variableName - The name of the field we want to set.
 * @param value - The value to be set to the field.
 * @param nonDefaultParam - Array that marks which non default parameters has been set.
 * @param msg - Pointer to the massage that represent the error that occurred.
 *
 * @return True after successful assignment or blank variableName.
 * 		   False otherwise.
 */
bool setConfigParameters(const SPConfig config,const char* variableName,const char* value,bool* nonDefaultParam, SP_CONFIG_MSG* msg) {
	if (strcmp(variableName,"\0") == 0) {
		return true;
	}
	if (strcmp(variableName,"spImagesDirectory") == 0) {
		config->spImagesDirectory = (char*) malloc(strlen(value)+1);
		if (config->spImagesDirectory == NULL){
			*msg = SP_CONFIG_ALLOC_FAIL;
			return false;
		}
		strcpy(config->spImagesDirectory, value);
		*nonDefaultParam = true;
	} else if (strcmp(variableName,"spImagesPrefix") == 0) {
		config->spImagesPrefix = (char*) malloc(strlen(value)+1);
		if (config->spImagesPrefix == NULL){
			*msg = SP_CONFIG_ALLOC_FAIL;
			return false;
		}
		strcpy(config->spImagesPrefix, value);
		*(nonDefaultParam+1) = true;
	} else if (strcmp(variableName,"spImagesSuffix") == 0) {
		if ((strcmp(value,".jpg") || strcmp(value,".png") || strcmp(value,".bmp") || strcmp(value,".gif")) != false) {
			config->spImagesSuffix = (char*) malloc(strlen(value)+1);
			if (config->spImagesSuffix == NULL){
				*msg = SP_CONFIG_ALLOC_FAIL;
				return false;
			}
			strcpy(config->spImagesSuffix, value);
			*(nonDefaultParam+2) = true;
		} else {
			*msg = SP_CONFIG_INVALID_STRING;
			return false;
		}
	} else if (strcmp(variableName,"spNumOfImages") == 0) {
		if (atoi(value) > 0) {
			config->spNumOfImages = atoi(value);
			*(nonDefaultParam+3) = true;
		} else {
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		}
	} else if (strcmp(variableName,"spPCADimension") == 0) {
		if (atoi(value) > 28 || atoi(value) < 10) {
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		} else {
			config->spPCADimension = atoi(value);
		}
	} else if (strcmp(variableName,"spPCAFilename") == 0) {
		config->spPCAFilename = (char*) malloc(strlen(value)+1);
		if (config->spPCAFilename == NULL){
			*msg = SP_CONFIG_ALLOC_FAIL;
			return false;
		}
		strcpy(config->spPCAFilename, value);
	} else if (strcmp(variableName,"spNumOfFeatures") == 0) {
		if (atoi(value) > 0) {
			config->spNumOfFeatures = atoi(value);
		} else {
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		}
	} else if (strcmp(variableName,"spExtractionMode") == 0) {
		if (strcmp(value,"true") == 0) {
			config->spExtractionMode = true;
		} else if (strcmp(value,"false") == 0) {
			config->spExtractionMode = false;
		} else {
			*msg = SP_CONFIG_INVALID_STRING;
			return false;
		}
	} else if (strcmp(variableName,"spNumOfSimilarImages") == 0) {
		if (atoi(value) > 0) {
			config->spNumOfSimilarImages = atoi(value);
		} else {
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		}
	} else if (strcmp(variableName,"spKDTreeSplitMethod") == 0) {
		if (strcmp(variableName,"RANDOM") == 0) {
			config->spKDTreeSplitMethod = RANDOM;
		} else if (strcmp(variableName,"MAX_SPREAD") == 0) {
			config->spKDTreeSplitMethod = MAX_SPREAD;
		} else if (strcmp(variableName,"INCREMENTAL") == 0) {
			config->spKDTreeSplitMethod = INCREMENTAL;
		} else {
			*msg = SP_CONFIG_INVALID_STRING;
			return false;
		}
	} else if (strcmp(variableName,"spKNN") == 0) {
		if (atoi(value) > 0) {
			config->spKNN = atoi(value);
		} else {
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		}
	} else if (strcmp(variableName,"spMinimalGUI") == 0) {
		if (strcmp(value,"true") == 0) {
			config->spMinimalGUI = true;
		} else if (strcmp(value,"false") == 0) {
			config->spMinimalGUI = false;
		} else {
			*msg = SP_CONFIG_INVALID_STRING;
			return false;
		}
	} else if (strcmp(variableName,"spLoggerLevel") == 0) {
		if (atoi(value) > 4 || atoi(value) < 1) {
			*msg = SP_CONFIG_INVALID_INTEGER;
			return false;
		} else {
			config->spLoggerLevel = atoi(value);
		}
	} else if (strcmp(variableName,"spLoggerFilename") == 0) {
		config->spLoggerFilename = (char*) malloc(strlen(value)+1);
		if (config->spLoggerFilename == NULL){
			*msg = SP_CONFIG_ALLOC_FAIL;
			return false;
		}
		strcpy(config->spLoggerFilename, value);
	} else {
		*msg = SP_CONFIG_INVALID_LINE;
		return false;
	}
	return true;
}
/*
 * The function return the index of the next non whitespace or tab character such that: index >= lineC.
 *
 * @param line - The string we are parsing.
 * @param lineC - The index of the character we are at.
 *
 * @return The index of the next non whitespace and tab character in line.
 */
int skipTabsAndWhitespace(const char* line, int lineC) {
	while (line[lineC] == ' ' || line[lineC] == '\t') { // skip tabs and whitespaces
		lineC++;
	}
	return lineC;
}

/*
 * The function Parse a config line and extract the variableName and value in this config line.
 *
 * @param line - The line we want to parse.
 * @param variableName - Pointer to the string we will store the name of the field we want to set at.
 * @param value - Pointer to the string we will store the value to be set to the field at.
 * @param msg - Pointer to the massage that represent the error that occurred.
 *
 * @return True if the line is valid configuration line, variableName and value contain valid values.
 * 		   False if the line is invalid configuration line, msg will be set with the right error msg.
 *
 */
bool ParseLine(char* line, char* variableName, char* value, SP_CONFIG_MSG* msg) {
	// Function variables
	int lineC = 0, varC = 0, valC = 0;
	// Function code
	lineC = skipTabsAndWhitespace(line, lineC);
	if (line[lineC] == '#' || line[lineC] == '\n') { // comment or end of line -> ignore line
		*variableName = '\0';
		*value = '\0';
		return true;
	}
	if (line[lineC] == '=') { // variable name is empty
		*msg = SP_CONFIG_INVALID_LINE;
		return false;
	}
	while ((line[lineC] != ' ') && (line[lineC] != '\t') && (line[lineC] != '=')) { // copy variable name
		variableName[varC] = line[lineC];
		lineC++;
		varC++;
	}
	variableName[varC] = '\0';
	lineC = skipTabsAndWhitespace(line, lineC);
	if (line[lineC] != '=') { // '=' character must separate the variable name and the value
		*msg = SP_CONFIG_INVALID_LINE;
		return false;
	}
	lineC++;
	lineC = skipTabsAndWhitespace(line, lineC);
	while (line[lineC] != ' ' && line[lineC] != '\t' && line[lineC] != '\n') { // copy variable value
		value[valC] = line[lineC];
		lineC++;
		valC++;
	}
	value[valC] = '\0';
	lineC = skipTabsAndWhitespace(line,lineC);
	if (line[lineC] != '\n') { // '\n' character must be the only character after the value string
		*msg = SP_CONFIG_INVALID_LINE;
		return false;
	}
	return true;
}
/*
 * The function free the memory of all the ParseConfig variables.
 *
 * If any parameter is NULL pointer (allocation fail or never allocated), then the function ignore it.
 */
void FreeParseConfig(char* buffer, char* line, char* value, char* variableName) {
	if (variableName)
		free(variableName);
	if (value)
		free(value);
	if (buffer)
		free(buffer);
	if (line)
		free(line);
}
/*
 * The function remove all the occurrences of a given character from a given string
 *
 * @param str - a pointer to the string we want to modify
 * @param c - the character we want to remove from the string
 */
void removeAllCharOccurrences(char* str, char c) {
    char *pr = str, *pw = str; // read point and write pointer
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c); // write over the given character
    }
    *pw = '\0';
}
/*
 * The function gets a configuration file and a configuration structure and set the structure with
 * the data in the file.
 *
 * @param configFile - Pointer to the configuration file.
 * @param config - The configuration structure.
 * @param msg - Pointer to the massage that represent the return msg (error or success).
 * @param lineNumber - The number of the last line that got parsed by the function.
 *
 * If the parsing ended successfully, then lineNumber contains the number of the last line in the file.
 * Else lineNumber contain the line number where the error occurred.
 *
 * @return True if the file is valid and was parsed successfully.
 * 	 	   False if the Parsing failed for any reason.
 */
bool ParseConfig(FILE* configFile, const SPConfig config, SP_CONFIG_MSG* msg, int* lineNumber) {
	// Function variables
	char* buffer = NULL, *line = NULL, *variableName = NULL, *value = NULL;
	int numOfChar = 0, i = 0, p = 0;
	bool nonDefaultParam[NON_DEFAULT_CONFIG_PARAM] = {0}; // init the array to false
	bool parseSuccess;
	// Memory allocation
	variableName = (char*) malloc(STRING_LENGTH);
	value = (char*) malloc(STRING_LENGTH);
	buffer = (char*) calloc(BUFFER_SIZE,sizeof(char));
	line = (char*) calloc(STRING_LENGTH,sizeof(char));
	if (variableName == NULL || value == NULL || buffer == NULL || line == NULL) {
		*msg = SP_CONFIG_ALLOC_FAIL;
		FreeParseConfig(buffer,line,value,variableName);
		return false;
	}
	// Function code
	while ((numOfChar = fread(buffer,sizeof(char),BUFFER_SIZE,configFile)) > 0) {
		for (i=0;i<numOfChar;i++) {
			*(line+p) = *(buffer+i); // copy a line from the buffer
			p++;
			if (*(buffer+i)=='\n') {
				removeAllCharOccurrences(line,'\r'); // delete all the \r characters
				parseSuccess = ParseLine(line,variableName,value, msg); // parse the line
				if (parseSuccess == false) { // invalid line error
					FreeParseConfig(buffer,line,value,variableName);
					return false;
				}
				parseSuccess = setConfigParameters(config,variableName,value,nonDefaultParam, msg); // set the field
				if (parseSuccess == false) { // bad variable name or invalid value error
					FreeParseConfig(buffer,line,value,variableName);
					return false;
				}
				(*lineNumber)++; // line counter
				p = 0;
			}
		}
	}
	for (i=0;i<NON_DEFAULT_CONFIG_PARAM;i++) {
		if (nonDefaultParam[i] == false) { // a non default parameter is not set in the configuration file
			switch (i) {
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

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg) {
	// Function variables
	SPConfig config;
	FILE* configFile;
	bool success;
	int lineNumber = 1;
	char errorMsg[STRING_LENGTH];
	// File opening and memory allocation
	if (filename == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		ConfigErrorMsg(filename,lineNumber,msg,errorMsg);
		PRINT(errorMsg);
		return NULL;
	}
	configFile = fopen(filename,"r");
	if (configFile == NULL) { // failed to open the file
		if (strcmp(filename,"spcbir.config") == 0)
			*msg = SP_CONFIG_DEFAULT_CANNOT_OPEN_FILE;
		else
			*msg = SP_CONFIG_CANNOT_OPEN_FILE;
		ConfigErrorMsg(filename,lineNumber,msg,errorMsg);
		PRINT(errorMsg);
		return NULL;
	}
	config = (SPConfig) malloc(sizeof(struct sp_config_t));
	if (config == NULL) {
		*msg = SP_CONFIG_ALLOC_FAIL;
		ConfigErrorMsg(filename,lineNumber,msg,errorMsg);
		PRINT(errorMsg);
		fclose(configFile);
		return NULL;
	}
	// Function code
	SetDefaultConfigValues(config); // set default values to config
	success = ParseConfig(configFile,config,msg,&lineNumber); // parse config file
	if (!success || !SetDefaultConfigString(config, msg)) { // set default strings if not set
		spConfigDestroy(config); // configuration file parsing failed
		ConfigErrorMsg(filename,lineNumber,msg,errorMsg);
		PRINT(errorMsg);
		fclose(configFile);
		return NULL;
	}
	*msg = SP_CONFIG_SUCCESS;
	fclose(configFile);
	return config;
}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	} else {
		*msg = SP_CONFIG_SUCCESS;
		return config->spExtractionMode;
	}
}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return false;
	} else {
		*msg = SP_CONFIG_SUCCESS;
		return config->spMinimalGUI;
	}
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	} else{
		*msg = SP_CONFIG_SUCCESS;
	}
	return config->spNumOfImages;
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	} else {
		*msg = SP_CONFIG_SUCCESS;
	}
	return config->spNumOfFeatures;
}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	} else {
		*msg = SP_CONFIG_SUCCESS;
	}
	return config->spPCADimension;
}

SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,int index) {
	if (imagePath == NULL || config == NULL) {
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	if (index >= config->spNumOfImages) {
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	}
	snprintf(imagePath,STRING_LENGTH,"%s%s%d%s",config->spImagesDirectory,config->spImagesPrefix,index,config->spImagesSuffix);
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetFeatsPath(char* imagePath, const SPConfig config, int index) {
	if (imagePath == NULL || config == NULL) {
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	if (index >= config->spNumOfImages) {
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	}
	snprintf(imagePath,STRING_LENGTH,"%s%s%d%s",config->spImagesDirectory,config->spImagesPrefix,index,".feats");
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config) {
	if (pcaPath == NULL || config == NULL) {
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	snprintf(pcaPath,STRING_LENGTH,"%s%s",config->spImagesDirectory,config->spPCAFilename);
	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetKDTreeSplitMethod(SP_SPLIT_METHOD* splitMethod, const SPConfig config) {
	if (splitMethod == NULL || config == NULL) {
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	*splitMethod = config->spKDTreeSplitMethod;
	return SP_CONFIG_SUCCESS;
}

int spConfigGetKNN(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	} else {
		*msg = SP_CONFIG_SUCCESS;
	}
	return config->spKNN;
}

int spConfigGetLoggerLevel(const SPConfig config, SP_CONFIG_MSG* msg) {
	assert(msg != NULL);
	if (config == NULL) {
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	} else {
		*msg = SP_CONFIG_SUCCESS;
	}
	return config->spLoggerLevel;
}

SP_CONFIG_MSG spConfigGetLoggerFileName(char* fileName, const SPConfig config) {
	if (fileName == NULL || config == NULL) {
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	snprintf(fileName,STRING_LENGTH,"%s",config->spLoggerFilename);
	return SP_CONFIG_SUCCESS;
}

void spConfigDestroy(SPConfig config) {
	if (config != NULL) {
		free(config->spImagesDirectory);
		free(config->spImagesPrefix);
		free(config->spImagesSuffix);
		free(config->spPCAFilename);
		free(config->spLoggerFilename);
		free(config);
	}
}
