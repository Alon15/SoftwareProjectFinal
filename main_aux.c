#include "SPConfig.h"
#include "SPLogger.h"
#include "SPPoint.h"
#include "main_aux.h"
#include <stdlib.h> // malloc, free, NULL
#include <stdio.h> // FILE, stdout, fopen, fclose, sprintf, printf, fflush, stdout
#include <stdbool.h> // bool, true, false
#include <string.h> // strcmp

#define STRING_LENGTH 1025 // 1024 + \0

// error messages
#define GET_LOGGER_FILENAME_FAIL_ERROR "An error occurred - could not retrieve logger's filename from the configuration file"
#define LOGGER_LEVEL_INVALID_ERROR "An error occurred - logger level is invalid"
#define LOGGER_CREATE_FAIL_ERROR "An error occurred - could not initialize logger to file"
#define DEFAULT_LOGGER_CREATE_FAIL_ERROR "An error occurred - could not initialize logger to stdout"

void getFileName(char* filename, int argc, char** argv) {
	if (argc > 3) {
		filename = NULL;
	} else if ((argc == 3)&&(argv[1][0] == '-')&&(argv[1][1] == 'c')) { // Special configuration file specified by "-c <config_filename>"
		strcpy(filename,argv[2]);
	} else { // Use default configuration file
		strcpy(filename,"spcbir.config");
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
	SP_CONFIG_MSG config_msg = SP_CONFIG_SUCCESS;
	SP_LOGGER_LEVEL loggerLevel;
	char filename[STRING_LENGTH];
	config_msg = spConfigGetLoggerFileName(filename,config); // get the logger filename
	if (config_msg != SP_CONFIG_SUCCESS){
		spLoggerPrintError(GET_LOGGER_FILENAME_FAIL_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	loggerLevel = parseLoggerLevel(spConfigGetLoggerLevel(config, &config_msg)); // parse the logger level
	if (config_msg != SP_CONFIG_SUCCESS){
		spLoggerPrintError(LOGGER_LEVEL_INVALID_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	if (strcmp(filename,"stdout") == 0){ // check if stdout was chosen to be output stream
		if (spLoggerCreate(NULL,loggerLevel) != SP_LOGGER_SUCCESS){ // create a logger that writes to stdout
			spLoggerPrintError(DEFAULT_LOGGER_CREATE_FAIL_ERROR,__FILE__,__func__,__LINE__);
			return false;
		}
	} else {
		if (spLoggerCreate(filename,loggerLevel) != SP_LOGGER_SUCCESS){ // create a logger that writes to file
			spLoggerPrintError(LOGGER_CREATE_FAIL_ERROR,__FILE__,__func__,__LINE__);
			return false;
	}
	}
	spLoggerPrintInfo("Logger was successfully initialized");
	return true;
}
