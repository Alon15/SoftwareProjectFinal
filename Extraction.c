#include "SPConfig.h"
#include "SPLogger.h"
#include "SPPoint.h"
#include <stdlib.h> // malloc, free, NULL
#include <stdio.h> // FILE, stdout, fopen, fclose, sprintf, printf, fflush, stdout
#include <stdbool.h> // bool, true, false
#include <string.h> // strcmp

#define STRING_LENGTH 1025 // 1024 + \n
#define BUFFER_SIZE 1024
#define MAX_FEATURE_DIM 28
#define CORRUPTED_FILE_ERROR_LIMIT 10

// Error messages
#define WRITE_FILE_ERROR "An error occurred - could not write to file"
#define PATH_IS_NULL_ERROR "An error occurred - invalid path to file"
#define OPEN_FILE_FAIL_ERROR "An error occurred - could not open file"
#define STR_TO_INT_FAIL_WARNING "Feature corrupted - could not parse expected integer"
#define STR_TO_DOUBLE_FAIL_WARNING "Feature corrupted - could not parse expected double"
#define CORRUPTED_DATA_DIM_WARNING "Feature corrupted - Dimension and actual number coordinates does not match"
#define CORRUPTED_FILE_ERROR "An error occurred - could not extract features from file, big part of the data is invalid"
#define CORRUPTED_FILE_TOO_MANY_FEATURES "An error occurred - number of features parameter does not match the the actual features extracted"
#define MEMORY_ALLOCATION_ERROR "An error occurred - allocation failure"

bool WriteFeat(FILE* featsFile, SPPoint feature){
	char storedFeat[STRING_LENGTH];
	char dataTruc[50];
	size_t success;
	int i=0, j=0;
	sprintf(dataTruc,"%d,",spPointGetDimension(feature));
	sprintf(storedFeat+j,"%s",dataTruc);
	j += strlen(dataTruc);
	for (i=0;i<(spPointGetDimension(feature));i++) {
		sprintf(dataTruc,"%.4f,",spPointGetAxisCoor(feature,i));
		sprintf(storedFeat+j,"%s",dataTruc);
		j += strlen(dataTruc);
	}
	storedFeat[j-1] = '\n';
	success = fwrite(storedFeat,1,strlen(storedFeat),featsFile);
	if (success == 0) {
		spLoggerPrintError(WRITE_FILE_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	return true;
}

bool ExportFeats(const char* filename, SPPoint* feats, int numOfFeats){
	int i;
	FILE* featsFile;
	size_t success;
	char firstLine[STRING_LENGTH];
	char infoLoggerMsg[STRING_LENGTH];
	sprintf(infoLoggerMsg,"Features export of: %s started",filename);
	spLoggerPrintInfo(infoLoggerMsg);
	if (filename == NULL) {
		spLoggerPrintError(PATH_IS_NULL_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	featsFile = fopen(filename,"w");
	if (featsFile == NULL) {
		spLoggerPrintError(OPEN_FILE_FAIL_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	sprintf(firstLine,"%d,%d\n",numOfFeats,spPointGetIndex(*feats)); // number of features and index of the image
	success = fwrite(firstLine,1,strlen(firstLine),featsFile);
	if (success == 0) {
		spLoggerPrintError(WRITE_FILE_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	for (i=0;i<numOfFeats;i++) {
		if (!WriteFeat(featsFile,feats[i])){
			return false;
		}
	}
	fclose(featsFile);
	sprintf(infoLoggerMsg,"Features export of: %s ended",filename);
	spLoggerPrintInfo(infoLoggerMsg);
	return true;
}

bool ParseFeature(char* feature,int* dim,double* data, int line,const char* filename) {
	char storedDim[50], storedData[STRING_LENGTH];
	char* success;
	int i = 0, j = 0, featsExtracted = 0;
	while (feature[i] != ',') {
		storedDim[j] = feature[i];
		i++;
		j++;
	}
	storedDim[j] = '\0';
	*dim = strtol(storedDim,&success,10);
	if (storedDim >= success) {
		spLoggerPrintWarning(STR_TO_INT_FAIL_WARNING,filename,__func__,line);
		return false;
	}
	i++;
	while (feature[i] != '\n') {
		j = 0;
		while (feature[i] != ',' && feature[i] != '\n') {
			storedData[j] = feature[i];
			i++;
			j++;
		}
		storedData[j] = '\0';
		data[featsExtracted] = strtod(storedData,&success);
		if (storedData >= success) {
			spLoggerPrintWarning(STR_TO_DOUBLE_FAIL_WARNING,filename,__func__,line);
			return false;
		}
		featsExtracted++;
		if (feature[i] != '\n')
			i++;
	}
	if (featsExtracted != *dim) {
		spLoggerPrintWarning(CORRUPTED_DATA_DIM_WARNING,filename,__func__,line);
		return false;
	}
	return true;
}

void FreeParseFeats(char* buffer, char* feature, char* header, double* data) {
	if (buffer)
		free(buffer);
	if (feature)
		free(feature);
	if (header)
		free(header);
	if (data)
		free(data);
}

SPPoint* ParseFeats(FILE* featsFile,const char* filename, int* numOfFeats) {
	char* buffer = NULL, *feature = NULL, *header = NULL, *ptr;
	double* data = NULL;
	int numOfChar = 0, i = 0, p = 0, j = 0, ind, dim, featsExtracted = 0, line = 0, featsFailed = 0;
	SPPoint* features = NULL;
	bool parseSuccess, firstLine = true;
	data = (double*) malloc(MAX_FEATURE_DIM*sizeof(double));
	buffer = (char*) calloc(BUFFER_SIZE,sizeof(char));
	feature = (char*) calloc(STRING_LENGTH,sizeof(char));
	header = (char*) calloc(STRING_LENGTH,sizeof(char));
	if (buffer == NULL || feature == NULL || data == NULL || header == NULL){
		FreeParseFeats(buffer, feature, header, data);
		spLoggerPrintError(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	while ((numOfChar = fread(buffer,sizeof(char),BUFFER_SIZE,featsFile)) > 0) {
		for (i=0;i<numOfChar;i++) {
			*(feature+p) = *(buffer+i);
			p++;
			if (*(buffer+i) == '\n') {
				if (firstLine) {
					p = 0;
					firstLine = false;
					while (feature[p] != ',') {
						header[j] = feature[p];
						p++;
						j++;
					}
					header[j] = '\0';
					*numOfFeats = strtol(header, &ptr, 10);
					if (header >= ptr) {
						spLoggerPrintWarning(STR_TO_INT_FAIL_WARNING,filename,__func__,line);
						return NULL;
					}
					j = 0;
					while (feature[p] != '\n') {
						header[j] = feature[p];
						p++;
						j++;
					}
					header[j] = '\0';
					ind = strtol(header, &ptr, 10);
					if (header >= ptr) {
						spLoggerPrintWarning(STR_TO_INT_FAIL_WARNING,filename,__func__,line);
						return NULL;
					}
					features = (SPPoint*) malloc(sizeof(SPPoint)*(*numOfFeats));
					if (features == NULL) {
						spLoggerPrintError(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
						FreeParseFeats(buffer, feature, header, data);
						return NULL;
					}
				} else {
					parseSuccess = ParseFeature(feature,&dim,data,line,filename);
					if (parseSuccess == false) {
						featsFailed++;
						if (featsFailed > CORRUPTED_FILE_ERROR_LIMIT){
							spLoggerPrintError(CORRUPTED_FILE_ERROR,filename,__func__,0);
							FreeParseFeats(buffer, feature, header, data);
							// TODO free memory of features (the array, and all his elements) featsExtracted = number of elements to free
							// TODO should be defined in main_aux
						}
					} else {
						if(featsExtracted > *numOfFeats){
							spLoggerPrintError(CORRUPTED_FILE_TOO_MANY_FEATURES,__FILE__,__func__,__LINE__);
							FreeParseFeats(buffer, feature, header, data);
							// TODO free memory of features (the array, and all his elements) featsExtracted = number of elements to free
							// TODO should be defined in main_aux
							return NULL;
						}
						features[featsExtracted] = spPointCreate(data,dim,ind);
						featsExtracted++;
					}
				}
				line++;
				p = 0;
			}
		}
	}
	FreeParseFeats(buffer, feature, header, data);
	return features;
}

SPPoint* ImportFeats(const char* filename, int* numOfFeats) {
	FILE* featsFile;
	SPPoint* features;
	char infoLoggerMsg[STRING_LENGTH];
	sprintf(infoLoggerMsg,"Features import of: %s started",filename);
	spLoggerPrintInfo(infoLoggerMsg);
	if (filename == NULL) {
		spLoggerPrintError(PATH_IS_NULL_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	featsFile = fopen(filename,"r");
	if (featsFile == NULL) {
		spLoggerPrintError(OPEN_FILE_FAIL_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	features = ParseFeats(featsFile,filename,numOfFeats);
	fclose(featsFile);
	sprintf(infoLoggerMsg,"Features import of: %s ended",filename);
	spLoggerPrintInfo(infoLoggerMsg);
	return features;
}
