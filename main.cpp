#include "SPImageProc.h"
extern "C" {
#include "Extraction.h"
#include "main_aux.h"
#include "SPConfig.h"
#include "SPLogger.h"
#include "SPPoint.h"
#include "SPKDArray.h" // TODO DEBUG DELME
}
#include <cstdio> // TODO Change 'stdio.h' to 'cstdio' ? I'm not sure we're allowed to use c++ libraries
#include <cstdlib> // TODO Change 'stdlib.h' to 'cstdlib' ?
#include <cstring> // TODO Change 'string.h' to 'cstring' ?

#define STRING_LENGTH 1025 // 1024 + \0
#define QUERY_IMG_MSG "Please enter an image path:\n"
#define EXIT_MSG "Exiting...\n"

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
		// TODO Error already been printed at 'spConfigCreate'
		// TODO Error printed as 'regular message'
		// TODO Free memory (including the logger spLogger))
		// TODO Terminate the program
		return EXIT_FAILURE;
	}
	if(!initLogger(config)){
		//TODO print error
		return EXIT_FAILURE;
	}
	imageProc = new ImageProc(config);
	if (spConfigIsExtractionMode(config,&config_msg)) {
		numOfImages = spConfigGetNumOfImages(config,&config_msg);
		for (index=0;index<numOfImages;index++) {
			config_msg = spConfigGetImagePath(imagePath,config,index);
			if (config_msg != SP_CONFIG_SUCCESS) {
				//TODO error
			}
			featuresArray = imageProc->getImageFeatures(imagePath,index,&numOfFeats); //TODO can't move this function from the main because of imageProc
			if (featuresArray == NULL) {
				//TODO error
			}
			config_msg = spConfigGetFeatsPath(imagePath,config,index);
			if (config_msg != SP_CONFIG_SUCCESS) {
				//TODO error
			}
			if (ExportFeats(imagePath,featuresArray,numOfFeats)) {
				//TODO error
			}
		}
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
