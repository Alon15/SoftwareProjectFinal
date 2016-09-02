#include "SPConfig.h"
#include "SPLogger.h"
#include "SPImageProc.h"
#include "SPPoint.h"
#include <stdlib.h> // malloc, free, NULL
#include <stdio.h> // FILE, stdout, fopen, fclose, sprintf, printf, fflush, stdout
#include <stdbool.h> // bool, true, false
#include <string.h> // strcmp

#define STRING_LENGTH 1025 // 1024 + \n
#define BUFFER_SIZE 1024
#define MAX_FEATURE_DIM 28


//TODO check if i need to convert array expressions to pointer expressions
void WriteFeat(FILE* featsFile,SPPoint feature){
	char storedFeat[STRING_LENGTH];
	char dataTruc[50];
	size_t success;
	int i=0, j=0;
	sprintf(dataTruc, "%d", feature->dim);
	sprintf(storedFeat[j],"%s,",dataTruc);
	j += strlen(dataTruc);
	for (i=0;i<(feature->dim);i++){
		sprintf(dataTruc, "%.4f", feature->data[i]);
		sprintf(storedFeat[j],"%s,",dataTruc);
		j += strlen(dataTruc);
	}
	storedFeat[j-1] = '\0';
    success = fwrite(storedFeat,1, strlen(storedFeat),featsFile);
    if (success == 0) {
    	// TODO error
    }

}
bool ExportFeats(const char* path, SPPoint* feats, int numOfFeats){
	int i;
	FILE* featsFile;
	size_t success;
	char* firstLine;
	if (path == NULL){
		//TODO error
		return false;
	}
	featsFile = fopen(path,"w");
	if (featsFile == NULL){
		//TODO error
		return false;
	}
	sprintf(firstLine, "%d,%d\n",numOfFeats, feats->index);
    success = fwrite(firstLine,1, strlen(firstLine),featsFile);
    if (success == 0) {
    	// TODO error
    	return false;
    }
	for (i=0; i<numOfFeats;i++){
		WriteFeat(featsFile,feats[i]);
	}
	fclose(featsFile);
	return true;
}
bool ParseFeature(char* feature, int* dim, double* data){
	char* storedDim, *storedData, *success;
	int i = 0, j = 0, featsExtracted = 0;
	while (feature[i] != ','){
		storedDim[j] = feature[i];
		i++;
		j++;
	}
	storedDim[j] = '\0';
	*dim = strtol(storedDim,&success, 10);
	if (storedDim >= success){
		//TODO error, failed to convert
		return false;
	}
	while (feature[i] != '\n'){
		j = 0;
		while (feature[i] != ','){
			storedData[j] = feature[i];
			i++;
			j++;
		}
		storedDim[j] = '\0';
		data[featsExtracted] = strtod(storedData,&success);
		if (storedData >= success){
			//TODO error, failed to convert
			return false;
		}
		featsExtracted++;
		i++;
	}
	if (featsExtracted != dim){
		//TODO error corrupted data
		return false;
	}
	return true;
}
SPPoint* ParseFeats(FILE* featsFile,int* numOfFeats){
	char* buffer, *feature, *header;
	double* data;
	int numOfChar = 0, i = 0, p = 0,j = 0, index, dim, featsExtracted = 0;
	SPPoint* features;
	bool parseSuccess, firstLine = true;

	data = (double*) malloc(MAX_FEATURE_DIM*sizeof(double));
	buffer = (char*) calloc(BUFFER_SIZE,sizeof(char));
	feature = (char*) calloc(STRING_LENGTH,sizeof(char));
	header = (char*) calloc(STRING_LENGTH,sizeof(char));
	if (buffer == NULL || feature == NULL || data == NULL || header == NULL){
		// TODO free memory
	}
	while ((numOfChar = fread(buffer,sizeof(char),BUFFER_SIZE,featsFile)) > 0){
		for (i=0;i<numOfChar;i++){
			*(feature+p) = *(buffer+i);
			p++;
			if (*(buffer+i) == '\n'){
				if (firstLine){
					p = 0;
					firstLine = false;
					while (feature[p] != ','){
						header[j] = feature[p];
						p++;
						j++;
					}
					header[j] = '\0';
					*numOfFeats = strtol(header);
					j = 0;
					while (feature[p] != '\n'){
						header[j] = feature[p];
						p++;
						j++;
					}
					header[j] = '\0';
					index = strtol(header);
					features = (SPPoint*) malloc(sizeof(SPPoint)*(*numOfFeats));
					if (features == NULL){
						// TODO free memory
					}
				}
				else {
				parseSuccess = ParseFeature(feature,&dim,data);
				if (parseSuccess == false){
					// TODO free memory
					// TODO count how many failed
				}
				features[featsExtracted] = spPointCreate(data,dim,index);
				featsExtracted++;
				}
				p = 0;
			}
		}

	}
	return features;
}

SPPoint* ImportFeats(const char* path, int* numOfFeats){
	FILE* featsFile;
	SPPoint* features;
	if (path == NULL){
		//TODO error
		return NULL;
	}
	featsFile = fopen(path,"r");
	if (featsFile == NULL){
		//TODO error
		return NULL;
	}
	features = ParseFeats(featsFile, &numOfFeats);

	fclose(featsFile);
	return features;
}
