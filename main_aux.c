#include "SPConfig.h"
#include "SPLogger.h"
#include "SPPoint.h"
#include "main_aux.h"
#include "defines.h"
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
	SPPoint p[] = {p0,p1,p2,p3,p4};
	SPKDArray k = Init(p,5);
	//int dim = k->dim;
	printf("OK\n");
	printf("A: %d\n",spPointGetIndex(p[0]));
	printf("B: %d\n",spPointGetDimension(p[0]));
	fflush(stdout);
	int d = spKDArrayGetDimension(k);
	int s = spKDArrayGetSize(k);
	printf("C: %d\n",d);
	printf("D: %d\n",s);
	fflush(stdout);
	int** mtrx = spKDArrayGetMatrix(k);
	for (int i=0;i<s;i++) {
		for (int j=0;j<d;j++) {
			printf("E: matrix[%d][%d] = %d\n",j,i,mtrx[j][i]);
		}
	}
	fflush(stdout);
}
