#include <cstdlib> // NULL
#include <cstring> // strcmp
#include "SPImageProc.h"
extern "C" {
#include "Extraction.h"
#include "main_aux.h"
#include "SPConfig.h"
#include "SPLogger.h"
#include "SPPoint.h"
#include "defines.h"
#include "KDTreeNode.h"
}

using namespace sp;

// C function that use C++ file (SPImageProc)
bool extractionMode(SPConfig config,ImageProc* imageProc) {
	// Function variables
	int index, numOfImages;
	char imagePath[STRING_LENGTH];
	SP_CONFIG_MSG config_msg = SP_CONFIG_SUCCESS;
	int numOfFeats;
	SPPoint* featuresArray = NULL;
	// Function code
	PRINT_INFO_LOGGER(EXTRACTION_MODE_START);
	numOfImages = spConfigGetNumOfImages(config,&config_msg);
	if (config_msg != SP_CONFIG_SUCCESS) {
		PRINT_ERROR_LOGGER(GET_NUM_OF_IMAGES_FAIL_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	for (index=0;index<numOfImages;index++) {
		config_msg = spConfigGetImagePath(imagePath,config,index);
		if (config_msg != SP_CONFIG_SUCCESS) {
			PRINT_ERROR_LOGGER(GET_IMAGE_PATH_FAIL_ERROR,__FILE__,__func__,__LINE__);
			return false;
		}
		featuresArray = imageProc->getImageFeatures(imagePath,index,&numOfFeats);
		if (featuresArray == NULL) {
			PRINT_ERROR_LOGGER(FEATURES_EXTRACTION_FROM_IMAGE_FAIL_ERROR,__FILE__,__func__,__LINE__);
			return false;
		}
		config_msg = spConfigGetFeatsPath(imagePath,config,index);
		if (config_msg != SP_CONFIG_SUCCESS) {
			PRINT_ERROR_LOGGER(GET_FEATS_PATH_FAIL_ERROR,__FILE__,__func__,__LINE__);
			return false;
		}
		if (!ExportFeats(imagePath,featuresArray,numOfFeats)) {
			return false;
		}
	}
	FREE_FEATURES_ARRAY(featuresArray,numOfFeats);
	PRINT_INFO_LOGGER(EXTRACTION_MODE_SUCCESS);
	return true;
}

// C function that use C++ file (SPImageProc)
bool showImages(SPConfig config,ImageProc* imageProc,int* closestImages,char* query) {
	// Function variables
	char imagePath[STRING_LENGTH];
	SP_CONFIG_MSG config_msg = SP_CONFIG_SUCCESS;
	int i, numOfSimilarImages;
	bool minimalGui;
	// Function code
	PRINT_INFO_LOGGER(SHOW_SIMILAR_IMAGES);
	minimalGui = spConfigMinimalGui(config,&config_msg);
	if (config_msg != SP_CONFIG_SUCCESS) {
		PRINT_ERROR_LOGGER(GET_GUI_FAIL_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	numOfSimilarImages = spConfigGetNumOfSimilarImages(config,&config_msg);
	if (config_msg != SP_CONFIG_SUCCESS) {
		PRINT_ERROR_LOGGER(GET_NUM_OF_IMAGES_FAIL_ERROR,__FILE__,__func__,__LINE__);
		return false;
	}
	if (!minimalGui) {
		PRINT_NON_MINIMAL_GUI_MSG(query);
	}
	for (i=0;i<numOfSimilarImages;i++) {
		config_msg = spConfigGetImagePath(imagePath,config,closestImages[i]);
		if (config_msg != SP_CONFIG_SUCCESS) {
			PRINT_ERROR_LOGGER(GET_IMAGE_PATH_FAIL_ERROR,__FILE__,__func__,__LINE__);
			return false;
		}
		if (!minimalGui)
			PRINT_NON_MINIMAL_GUI_RESULT(imagePath);
		else
			imageProc->showImage(imagePath);
	}
	return true;
}

// C function that use C++ file (SPImageProc)
bool query(SPConfig config, ImageProc* imageProc, KDTreeNode kdTree) {
	// Function variables
	char query[STRING_LENGTH] = {'\0'};
	int numOfFeats = 0;
	int* closestImages;
	SPPoint* featuresArray = NULL;
	// Function code
	while (true) {
		PRINT(QUERY_IMG_MSG);
		scanf("%1024s",query);
		if(strcmp(query,"<>") == 0) {
			break;
		}
		if (fileCheck(query)) {
			featuresArray = imageProc->getImageFeatures(query,0,&numOfFeats);
			if (featuresArray == NULL) {
				PRINT_ERROR_LOGGER(FEATURES_EXTRACTION_FROM_IMAGE_FAIL_ERROR,__FILE__,__func__,__LINE__);
				return false;
			}
			PRINT_INFO_LOGGER(SEARCH_FOR_SIMILAR_IMAGES);
			closestImages = closestImagesQuery(config,kdTree,featuresArray,numOfFeats); // find the closest images
			if (closestImages == NULL) {
				return false;
			}
			PRINT_INFO_LOGGER(SIMILAR_IMAGES_FOUND);
			FREE_FEATURES_ARRAY(featuresArray,numOfFeats); // free the query features
			if(!showImages(config,imageProc,closestImages,query)){
				free(closestImages);
				return false;
			}
			free(closestImages);
		}
	}
	return true;
}

int main (int argc, char *argv[]) {
	// Function variables
	char filename[STRING_LENGTH];
	int numOfFeats = 0;
	SP_CONFIG_MSG config_msg = SP_CONFIG_SUCCESS;
	KDTreeNode kdTree = NULL;
	SPConfig config;
	ImageProc* imageProc = NULL;
	SPPoint* featuresArray = NULL;
	// Function code
	getFileName(filename,argc,argv);
	config = spConfigCreate(filename, &config_msg); // Load the configuration file
	if (config_msg != SP_CONFIG_SUCCESS) {
		return EXIT_FAILURE;
	}
	if (!initLogger(config)) { // initialize the logger
		FREE_CONFIG(config);
		return EXIT_FAILURE;
	}
	PRINT_INFO_LOGGER(IMAGE_PROC_INIT);
	imageProc = new ImageProc(config); // initialize imageProc
	PRINT_INFO_LOGGER(IMAGE_PROC_SUCCESS);
	if (spConfigIsExtractionMode(config,&config_msg)) { // Extraction mode
		if (!extractionMode(config,imageProc)) {
			FREE_ALL(config,featuresArray,numOfFeats,kdTree)
			return EXIT_FAILURE;
		}
	}
	PRINT_INFO_LOGGER(EXTRACT_FEATURES_START);
	featuresArray = extractAllFeatures(config,&numOfFeats);
	if (featuresArray == NULL) {
		FREE_ALL(config,featuresArray,numOfFeats,kdTree)
		return EXIT_FAILURE;
	}
	PRINT_INFO_LOGGER(EXTRACT_FEATURES_SUCCESS);
	PRINT_INFO_LOGGER(KDTREE_INIT);
	if (!spKDTreeInit(config,featuresArray,numOfFeats,&kdTree)) { // initialize KDTree
		FREE_ALL(config,featuresArray,numOfFeats,kdTree)
		return EXIT_FAILURE;
	}
	PRINT_INFO_LOGGER(KDTREE_SUCCESS);
	PRINT_INFO_LOGGER(QUERY_START);
	if (!query(config,imageProc,kdTree)) { // start query
		FREE_ALL(config,featuresArray,numOfFeats,kdTree)
		return (EXIT_SUCCESS);
	}
	PRINT(EXIT_MSG);
	FREE_ALL(config,featuresArray,numOfFeats,kdTree)
	return (EXIT_SUCCESS);
}
