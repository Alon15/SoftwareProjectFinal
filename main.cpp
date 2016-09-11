#include "SPImageProc.h"
extern "C" {
#include "Extraction.h"
#include "main_aux.h"
#include "SPConfig.h"
#include "SPLogger.h"
#include "SPPoint.h"
#include "defines.h"
}
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace sp;

bool extractionMode(SPConfig config,SPPoint* featuresArray,ImageProc* imageProc,SP_CONFIG_MSG config_msg,int* numOfFeats){
	int index, numOfImages;
	char imagePath[STRING_LENGTH];
	spLoggerPrintInfo(EXTRACTION_MODE_START);
	numOfImages = spConfigGetNumOfImages(config,&config_msg);
	for (index=0;index<numOfImages;index++) {
		config_msg = spConfigGetImagePath(imagePath,config,index);
		if (config_msg != SP_CONFIG_SUCCESS) {
			spLoggerPrintError(GET_IMAGE_PATH_FAIL_ERROR,__FILE__,__func__,__LINE__);
			return false;
		}
		featuresArray = imageProc->getImageFeatures(imagePath,index,numOfFeats);
		if (featuresArray == NULL) {
			spLoggerPrintError(FEATURES_EXTRACTION_FROM_IMAGE_FAIL_ERROR,__FILE__,__func__,__LINE__);
			return false;
		}
		config_msg = spConfigGetFeatsPath(imagePath,config,index);
		if (config_msg != SP_CONFIG_SUCCESS) {
			spLoggerPrintError(GET_FEATS_PATH_FAIL_ERROR,__FILE__,__func__,__LINE__);
			return false;
		}
		if (!ExportFeats(imagePath,featuresArray,*numOfFeats)) {
			return false;
		}
	}
	FREE_FEATURES_ARRAY(featuresArray,*numOfFeats);
	spLoggerPrintInfo(EXTRACTION_MODE_SUCCESS);
	return true;
}

int main (int argc, char *argv[]) {
	//tmpFunc1(); //TODO it cause the program to crash, probably memory issue
	char filename[STRING_LENGTH], query[STRING_LENGTH] = {'\0'};
	SP_CONFIG_MSG config_msg = SP_CONFIG_SUCCESS;
	SPConfig config;
	ImageProc* imageProc = NULL;
	int numOfFeats;
	SPPoint* featuresArray = NULL;
	getFileName(filename,argc,argv);
	config = spConfigCreate(filename, &config_msg); // Load the configuration file
	if (config_msg != SP_CONFIG_SUCCESS) {
		return EXIT_FAILURE;
	}
	if(!initLogger(config)){ // initialize the logger
		FREE_CONFIG(config);
		return EXIT_FAILURE;
	}
	imageProc = new ImageProc(config); // initialize imageProc
	spLoggerPrintInfo(IMAGE_PROC_SUCCESS);
	if (spConfigIsExtractionMode(config,&config_msg)) { // Extraction mode
		if(!extractionMode(config, featuresArray, imageProc,config_msg,&numOfFeats)){
			FREE_ALL(config,featuresArray,numOfFeats)
			return EXIT_FAILURE;
		}
	}
	// TODO import the extracted features
	// TODO build the KDtree
	while (strcmp(query,"<>") != 0) { //TODO move the query loop to main_aux or to new function in main.cpp
		PRINT(QUERY_IMG_MSG); //TODO figure out why this line is red (does not crash the program or cause compilation error)
		scanf("%1024s",query);
		//TODO check if the query path is valid
		//TODO get features of the new image
		//TODO find matches (KDTree search)
		//TODO show matches (GUI or stdout)
		//TODO reset variable of the current query (clear the features)
	}
	PRINT(EXIT_MSG);
	FREE_ALL(config,featuresArray,numOfFeats)
	return (EXIT_SUCCESS);
}
