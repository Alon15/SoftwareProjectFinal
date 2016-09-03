#include <stdio.h> // TODO Change 'stdio.h' to 'cstdio' ?
#include <stdlib.h> // TODO Change 'stdlib.h' to 'cstdlib' ?
#include <string.h> // TODO Change 'string.h' to 'cstring' ?
#include "SPImageProc.h"
extern "C" {
#include "Extraction.h"
#include "main_aux.h"
#include "SPConfig.h"
}

#define STRING_LENGTH 1025 // 1024 + \0
#define QUERY_IMG_MSG "Please enter an image path:\n"
#define EXIT_MSG "Exiting...\n"

using namespace sp;

int main (int argc, char *argv[]) {
	char* filename, *imagePath;
	char query[STRING_LENGTH] = {'\0'};
	SP_CONFIG_MSG config_msg = SP_CONFIG_SUCCESS;
	SPConfig config;
	ImageProc *imageProc = NULL;
	int index, numOfImages, numOfFeats;
	SPPoint* featuresArray;
	if (argc > 2) {
		filename = NULL;
	} else if (argc == 2) {
		filename = argv[1];
	} else {
		filename = (char*)"spcbir.config";
	config = spConfigCreate(filename, &config_msg);
	if (config_msg != SP_CONFIG_SUCCESS) {
		//TODO error
	}
	// TODO activate the logger
	try {
		imageProc = new ImageProc(config);
	} catch(...) {
		//TODO ImageProc error, terminate
	}
	if (spConfigIsExtractionMode(config,&config_msg)) {
		numOfImages = spConfigGetNumOfImages(config,&config_msg);
		for (index=0;index<numOfImages;index++) {
			config_msg = spConfigGetImagePath(imagePath,config,index);
			if (config_msg != SP_CONFIG_SUCCESS) {
				//TODO error
			}
			featuresArray = imageProc->getImageFeatures(imagePath,index,&numOfFeats);
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
		query[strlen(query) - 1] = '\0'; // delete the \n from the end of the query
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
	return (EXIT_SUCCESS);
}
