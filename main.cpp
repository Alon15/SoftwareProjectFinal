#include "SPImageProc.h"
extern "C" {
#include "Extraction.h"
#include "main_aux.h"
#include "SPConfig.h"
#include "SPLogger.h"
#include "SPPoint.h"
#include "SPKDArray.h" // TODO DEBUG DELME
}
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define STRING_LENGTH 1025 // 1024 + \0
#define QUERY_IMG_MSG "Please enter an image path:\n"
#define EXIT_MSG "Exiting...\n"

// error messages
#define GET_IMAGE_PATH_FAIL_ERROR "An error occurred - could not retrieve image path from the configuration file"
#define GET_FEATS_PATH_FAIL_ERROR "An error occurred - could not retrieve feats path from the configuration file"
#define FEATURES_EXTRACTION_FROM_IMAGE_FAIL_ERROR "An error occurred - could not extract features from an image"

//TODO move all the error messages and defines to new file?
//TODO convery all the info messages literals to #define MSG format
using namespace sp;

int main (int argc, char *argv[]) {
	tmpFunc1();
	char filename[STRING_LENGTH], imagePath[STRING_LENGTH], query[STRING_LENGTH] = {'\0'};
	SP_CONFIG_MSG config_msg = SP_CONFIG_SUCCESS;
	SPConfig config;
	ImageProc *imageProc = NULL;
	int index, numOfImages, numOfFeats;
	SPPoint* featuresArray;
	getFileName(filename,argc,argv);
	config = spConfigCreate(filename, &config_msg); // Load the configuration file
	if (config_msg != SP_CONFIG_SUCCESS) {
		// TODO Errors already been printed at 'spConfigCreate', no memory release required
		return EXIT_FAILURE;
	}
	if(!initLogger(config)){
		return EXIT_FAILURE;
	}
	imageProc = new ImageProc(config);
	spLoggerPrintInfo("imageProc was successfully initialized");
	if (spConfigIsExtractionMode(config,&config_msg)) {
		spLoggerPrintInfo("Run extraction mode");
		numOfImages = spConfigGetNumOfImages(config,&config_msg);
		for (index=0;index<numOfImages;index++) {
			config_msg = spConfigGetImagePath(imagePath,config,index);
			if (config_msg != SP_CONFIG_SUCCESS) {
				spLoggerPrintError(GET_IMAGE_PATH_FAIL_ERROR,__FILE__,__func__,__LINE__);
				//TODO free memory (logger + imageProc + config)
				return EXIT_FAILURE;
			}
			featuresArray = imageProc->getImageFeatures(imagePath,index,&numOfFeats);
			if (featuresArray == NULL) {
				spLoggerPrintError(FEATURES_EXTRACTION_FROM_IMAGE_FAIL_ERROR,__FILE__,__func__,__LINE__);
				//TODO free memory (logger + imageProc + config)
				return EXIT_FAILURE;
			}
			config_msg = spConfigGetFeatsPath(imagePath,config,index);
			if (config_msg != SP_CONFIG_SUCCESS) {
				spLoggerPrintError(GET_FEATS_PATH_FAIL_ERROR,__FILE__,__func__,__LINE__);
				//TODO free memory (logger + imageProc + config)
				return EXIT_FAILURE;
			}
			if (!ExportFeats(imagePath,featuresArray,numOfFeats)) {
				//TODO free memory (logger + imageProc + config + featuresArray and his elements)
				return EXIT_FAILURE;
			}
		}
		spLoggerPrintInfo("Extraction mode finished successfully");
	}
	// TODO import the extracted features
	// TODO build the KDtree
	while (strcmp(query,"<>") != 0) {
		printf(QUERY_IMG_MSG);
		fflush(stdout);
		scanf("%1024s",query);
		//TODO check if the query path is valid
		//TODO get features of the new image
		//TODO find matches (KDTree search)
		//TODO show matches (GUI or stdout)
		//TODO reset variable of the current query (clear the features)
	}
	printf(EXIT_MSG);
	fflush(stdout);
	//TODO free memory
	delete imageProc;
	spConfigDestroy(config);
	//TODO figure out why the program crash here instead of peacefully end
	return (EXIT_SUCCESS);
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
