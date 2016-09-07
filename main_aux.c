#include "SPConfig.h"
#include "SPLogger.h"
#include "SPPoint.h"
#include "main_aux.h"
#include <stdlib.h> // malloc, free, NULL
#include <stdio.h> // FILE, stdout, fopen, fclose, sprintf, printf, fflush, stdout
#include <stdbool.h> // bool, true, false
#include <string.h> // strcmp

#define STRING_LENGTH 1025 // 1024 + \0

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
	return NULL;
}
bool initLogger(SPConfig config){
	SP_CONFIG_MSG config_msg = SP_CONFIG_SUCCESS;
	SP_LOGGER_LEVEL loggerLevel;
	char filename[STRING_LENGTH];
	config_msg = spConfigGetLoggerFileName(filename,config);
	if (config_msg != SP_CONFIG_SUCCESS){
		//TODO print error
		return false;
	}
	loggerLevel = parseLoggerLevel(spConfigGetLoggerLevel(config, &config_msg));
	if (config_msg != SP_CONFIG_SUCCESS){
		//TODO print error
		return false;
	}
	if (spLoggerCreate(filename,loggerLevel) != SP_LOGGER_SUCCESS){
		//TODO print error
		return false;
	}
	//TODO print success msg?
	return true;
}
