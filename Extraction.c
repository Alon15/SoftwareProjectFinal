#include <stdlib.h> // malloc, calloc, free, NULL, strtod, strtol
#include <stdio.h> // FILE, fopen, fclose, sprintf, printf, fwrite
#include <stdbool.h> // bool, true, false
#include <string.h> // strlen
//#include "SPConfig.h" // TODO Can we delete this line?
#include "SPLogger.h"
#include "SPPoint.h"
#include "defines.h"
#include "main_aux.h"
//#include "Extraction.h" // TODO Can we delete this line?

/*
 * The function writes a SPPoint (feature) to a file
 *
 * The function save the data (of type double) in a precision of up to 4 decimal digits
 *
 * @param featsFile - the file we are write the feature to
 * @param feature - the feature we want to write to the file
 *
 * @return True if the feature was written to file successfully to file
 * 	 	   False if the writing failed
 */
bool WriteFeat(FILE* featsFile, SPPoint feature) {
	// Function variables
	char storedFeat[STRING_LENGTH], dataTruc[50];
	size_t success;
	int i=0, j=0;
	// Function code
	sprintf(dataTruc,"%d,",spPointGetDimension(feature)); // write dimension
	sprintf(storedFeat+j,"%s",dataTruc);
	j += strlen(dataTruc);
	for (i=0;i<(spPointGetDimension(feature));i++) {
		sprintf(dataTruc,"%.4f,",spPointGetAxisCoor(feature,i)); // write data
		sprintf(storedFeat+j,"%s",dataTruc);
		j += strlen(dataTruc);
	}
	storedFeat[j-1] = '\n';
	success = fwrite(storedFeat,1,strlen(storedFeat),featsFile); // write the line to file
	if (success == 0) {
		PRINT_ERROR_LOGGER(WRITE_FILE_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	return true;
}

bool ExportFeats(const char* filename, SPPoint* feats, int numOfFeats) {
	// Function variables
	int i;
	FILE* featsFile;
	size_t success;
	char firstLine[STRING_LENGTH], infoLoggerMsg[STRING_LENGTH];
	// Function code
	LOGGER_INFO_EXTRACTION_MSG(infoLoggerMsg,"Features export of: %s started",filename)
	if (filename == NULL) {
		PRINT_ERROR_LOGGER(PATH_IS_NULL_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	featsFile = fopen(filename,"w");
	if (featsFile == NULL) {
		PRINT_ERROR_LOGGER(OPEN_FILE_FAIL_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	sprintf(firstLine,"%d,%d\n",numOfFeats,spPointGetIndex(*feats)); // number of features and index of the image
	success = fwrite(firstLine,1,strlen(firstLine),featsFile);
	if (success == 0) {
		PRINT_ERROR_LOGGER(WRITE_FILE_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	for (i=0;i<numOfFeats;i++) {
		if (!WriteFeat(featsFile,feats[i])) { // write the features to the file
			return false;
		}
	}
	fclose(featsFile);
	LOGGER_INFO_EXTRACTION_MSG(infoLoggerMsg,"Features export of: %s ended",filename)
	return true;
}
/*
 * The function parse a feature and extract the dimension and data from the feature string
 *
 * @param feature - a string that represent a feature (extracted from '.feats' file)
 * @param dim - a pointer to an integer that will contain the extracted dimension
 * @param data - a pointer to a double array that will contain the extracted data
 * @param line - the line number of this feature in the file (used for messages)
 * @param filename - the path to the file this feature came from (used for messages)
 *
 * @return True if the feature was parsed successfully
 * 	 	   False if the parsing failed
 */
bool ParseFeature(char* feature,int* dim,double* data, int line,const char* filename) {
	// Function variables
	char storedDim[50], storedData[STRING_LENGTH];
	char* success;
	int i = 0, j = 0, coordExtracted = 0;
	// Function code
	while (feature[i] != ',') {
		storedDim[j] = feature[i]; // extract dimension
		i++;
		j++;
	}
	storedDim[j] = '\0';
	*dim = strtol(storedDim,&success,10); // assign dimension to dim
	if (storedDim >= success) {
		PRINT_WARNING_LOGGER(STR_TO_INT_FAIL_WARNING,filename,__func__,line);
		return false;
	}
	i++;
	while (feature[i] != '\n') {
		j = 0;
		while (feature[i] != ',' && feature[i] != '\n') {
			storedData[j] = feature[i]; // copy a single coordinate
			i++;
			j++;
		}
		storedData[j] = '\0';
		data[coordExtracted] = strtod(storedData,&success); // assign a coordinate to the data array
		if (storedData >= success) {
			PRINT_WARNING_LOGGER(STR_TO_DOUBLE_FAIL_WARNING,filename,__func__,line);
			return false;
		}
		coordExtracted++;
		if (feature[i] != '\n') {
			i++;
		}
	}
	if (coordExtracted != *dim) { // dimension is not equal to the number of coordinates extracted
		PRINT_WARNING_LOGGER(CORRUPTED_DATA_DIM_WARNING,filename,__func__,line);
		return false;
	}
	return true;
}

/*
 * The function free the memory of all the ParseFeats variables.
 *
 * If any parameter is NULL pointer (allocation fail or never allocated), then the function ignore it.
 */
void FreeParseFeats(char* buffer, char* feature, char* header, double* data) {
	if (buffer) { // A tiny chance for errors in some compilers
		free(buffer);
		buffer = NULL; // Preventing a "double-free"
	}
	if (feature) { // A tiny chance for errors in some compilers
		free(feature);
		feature = NULL; // Preventing a "double-free"
	}
	if (header) { // A tiny chance for errors in some compilers
		free(header);
		header = NULL; // Preventing a "double-free"
	}
	if (data) { // A tiny chance for errors in some compilers
		free(data);
		data = NULL; // Preventing a "double-free"
	}
}

/*
 * The function parse a features file and create from the data a new SPPoint array that contain all
 * the data from the file
 *
 * @param featsFile - the file we want to extract the data from
 * @param filename - the path to the file we want to parse (used for messages)
 * @param numOfFeats - a pointer to the number of features the imported array has
 *
 * @return a pointer to the extracted SPPoint array if the import completed successfully
 * 		   a NULL pointer if the import failed
 */
SPPoint* ParseFeats(FILE* featsFile,const char* filename, int* numOfFeats) {
	// Function variables
	char* buffer = NULL, *feature = NULL, *header = NULL, *ptr;
	double* data = NULL;
	int numOfChar = 0, i = 0, p = 0, j = 0, ind, dim, featsExtracted = 0, line = 0, featsFailed = 0;
	SPPoint* features = NULL;
	bool parseSuccess, firstLine = true;
	// Memory allocation
	data = (double*)malloc(MAX_FEATURE_DIM*sizeof(double));
	buffer = (char*)calloc(BUFFER_SIZE,sizeof(char));
	feature = (char*)calloc(STRING_LENGTH,sizeof(char));
	header = (char*)calloc(STRING_LENGTH,sizeof(char));
	if (buffer == NULL || feature == NULL || data == NULL || header == NULL) {
		FreeParseFeats(buffer, feature, header, data);
		PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	// Function code
	while ((numOfChar = fread(buffer,sizeof(char),BUFFER_SIZE,featsFile)) > 0) { // read the file
		for (i=0;i<numOfChar;i++) {
			*(feature+p) = *(buffer+i); // copy a line from the buffer
			p++;
			if (*(buffer+i) == '\n') {
				if (firstLine) { // the first line contains different information from the rest of the file
					p = 0;
					firstLine = false;
					while (feature[p] != ',') {
						header[j] = feature[p]; // copy the number of features in the file
						p++;
						j++;
					}
					header[j] = '\0';
					*numOfFeats = strtol(header, &ptr, 10);
					if (header >= ptr) {
						PRINT_WARNING_LOGGER(STR_TO_INT_FAIL_WARNING,filename,__func__,line);
						return NULL;
					}
					j = 0;
					p++;
					while (feature[p] != '\n') {
						header[j] = feature[p]; // copy the index of the image
						p++;
						j++;
					}
					header[j] = '\0';
					ind = strtol(header, &ptr, 10);
					if (header >= ptr) {
						PRINT_WARNING_LOGGER(STR_TO_INT_FAIL_WARNING,filename,__func__,line);
						return NULL;
					}
					// memory allocation
					features = (SPPoint*)malloc(sizeof(SPPoint)*(*numOfFeats));
					if (features == NULL) {
						PRINT_ERROR_LOGGER(MEMORY_ALLOCATION_ERROR,__FILE__,__func__,__LINE__);
						FreeParseFeats(buffer, feature, header, data);
						return NULL;
					}
				} else {
					parseSuccess = ParseFeature(feature,&dim,data,line,filename); // parse the line
					if (parseSuccess == false) {
						featsFailed++; // count how many feature failed to be extracted
						if (featsFailed > CORRUPTED_FILE_ERROR_LIMIT) { // too many features failed to be extracted
							PRINT_ERROR_LOGGER(CORRUPTED_FILE_ERROR,filename,__func__,0);
							FreeParseFeats(buffer, feature, header, data);
							freeMainMemory(NULL,features,featsExtracted,false);
						}
					} else {
						if(featsExtracted > *numOfFeats) { // checks if we are trying to extract more features then expected
							PRINT_ERROR_LOGGER(CORRUPTED_FILE_TOO_MANY_FEATURES,__FILE__,__func__,__LINE__);
							FreeParseFeats(buffer, feature, header, data);
							freeMainMemory(NULL,features,featsExtracted,false);
							return NULL;
						}
						features[featsExtracted] = spPointCreate(data,dim,ind); // create the feature
						featsExtracted++; // count how many feature successfully extracted
					}
				}
				line++; // count lines
				p = 0;
			}
		}
	}
	FreeParseFeats(buffer, feature, header, data);
	return features;
}

SPPoint* ImportFeats(const char* filename, int* numOfFeats) {
	// Function variables
	FILE* featsFile;
	SPPoint* features;
	char infoLoggerMsg[STRING_LENGTH];
	// Function code
	LOGGER_INFO_EXTRACTION_MSG(infoLoggerMsg,"Features import of: %s started",filename)
	if (filename == NULL) {
		PRINT_ERROR_LOGGER(PATH_IS_NULL_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	featsFile = fopen(filename,"r");
	if (featsFile == NULL) {
		PRINT_ERROR_LOGGER(OPEN_FILE_FAIL_ERROR,__FILE__,__func__,__LINE__);
		return NULL;
	}
	features = ParseFeats(featsFile,filename,numOfFeats);
	fclose(featsFile);
	LOGGER_INFO_EXTRACTION_MSG(infoLoggerMsg,"Features import of: %s ended",filename)
	return features;
}
