#ifndef DEFINES_H_
#define DEFINES_H_

/*
 * Constants
 */

// general
#define STRING_LENGTH 1025 // 1024 + \0
#define BUFFER_SIZE 1024 // Size of the file read/write buffer

// main and main_aux
#define DEFAULT_CONFIG_FILE "spcbir.config"

// Extraction
#define MAX_FEATURE_DIM 28
#define CORRUPTED_FILE_ERROR_LIMIT 10

// SPConfig
#define NON_DEFAULT_CONFIG_PARAM 4 // Number of non default parameters in the config file
#define CONFIG_PARAM 14 // Number of parameters in the config file

/*
 * Messages
 */

// main messages
#define GET_NUM_OF_IMAGES_FAIL_ERROR "An error occurred - could not retrieve the number of images from the configuration file"
#define GET_IMAGE_PATH_FAIL_ERROR "An error occurred - could not retrieve image path from the configuration file"
#define GET_FEATS_PATH_FAIL_ERROR "An error occurred - could not retrieve feats path from the configuration file"
#define FEATURES_EXTRACTION_FROM_IMAGE_FAIL_ERROR "An error occurred - could not extract features from an image"
#define GET_GUI_FAIL_ERROR "An error occurred - could not retrieve gui property from the configuration file"
#define PRINT_NON_MINIMAL_GUI_MSG(path) printf("Best candidates for - %s - are:\n",path)
#define PRINT_NON_MINIMAL_GUI_RESULT(path) printf("%s\n",path)
#define QUERY_IMG_MSG "Please enter an image path:\n"
#define QUERY_START "Getting queries from the user"
#define SEARCH_FOR_SIMILAR_IMAGES "Similar images search started"
#define SIMILAR_IMAGES_FOUND "Similar images found successfully"
#define SHOW_SIMILAR_IMAGES "Showing similar images"
#define LOGGER_PRINT_FAILURE "An error occurred - Logger print failure"
#define EXIT_MSG "Exiting...\n"

// Extraction messages
#define WRITE_FILE_ERROR "An error occurred - could not write to file"
#define PATH_IS_NULL_ERROR "An error occurred - invalid path to file"
#define OPEN_FILE_FAIL_ERROR "An error occurred - could not open file"
#define STR_TO_INT_FAIL_WARNING "Feature corrupted - could not parse expected integer"
#define STR_TO_DOUBLE_FAIL_WARNING "Feature corrupted - could not parse expected double"
#define CORRUPTED_DATA_DIM_WARNING "Feature corrupted - Dimension and actual number coordinates does not match"
#define CORRUPTED_FILE_ERROR "An error occurred - could not extract features from file, big part of the data is corrupted"
#define CORRUPTED_FILE_TOO_MANY_FEATURES "An error occurred - number of features parameter does not match the the actual features extracted"
#define MEMORY_ALLOCATION_ERROR "An error occurred - allocation failure"

// main_aux messages
#define GET_LOGGER_FILENAME_FAIL_ERROR "An error occurred - could not retrieve logger's filename from the configuration file"
#define LOGGER_LEVEL_INVALID_ERROR "An error occurred - logger level is invalid"
#define LOGGER_CREATE_FAIL_ERROR "An error occurred - could not initialize logger to file"
#define DEFAULT_LOGGER_CREATE_FAIL_ERROR "An error occurred - could not initialize logger to stdout"
#define FILE_NOT_EXISTS "Invalid query - file does not exists\n"
#define FILE_CANT_READ "Invalid query - file cannot be read\n"

// SPConfig messages
#define CONFIG_NON_DEFAULT_PARAMETER_MISSING(errorMsg,file,lineNum,param) snprintf(errorMsg,STRING_LENGTH,"File: %s\nLine: %d\nMessage: Parameter %s is not set\n",file,lineNum,param)
#define CONFIG_CANT_OPEN_FILE(errorMsg,filename) snprintf(errorMsg,STRING_LENGTH,"The configuration file %s couldn't be open\n",filename)
#define CONFIG_DEFAULT_CANT_OPEN_FILE(errorMsg) snprintf(errorMsg,STRING_LENGTH,"The default configuration file %s couldn't be open\n",DEFAULT_CONFIG_FILE)
#define CONFIG_ALLOC_FAIL(errorMsg) snprintf(errorMsg,STRING_LENGTH,"%s\n",MEMORY_ALLOCATION_ERROR)
#define CONFIG_INVALID_LINE(errorMsg,filename,lineNumber) snprintf(errorMsg,STRING_LENGTH,"File: %s\nLine: %d\nMessage: Invalid configuration line\n",filename, lineNumber)
#define CONFIG_INVALID_VALUE(errorMsg,filename,lineNumber) snprintf(errorMsg,STRING_LENGTH,"File: %s\nLine: %d\nMessage: Invalid value - constraint not met\n",filename, lineNumber)
#define CONFIG_INVALID_ARGUMENT(errorMsg) snprintf(errorMsg,STRING_LENGTH,"Invalid command line : use -c <config_filename>\n")

// KDTreeNode messages
#define KDTREE_IS_NULL "An error occurred - the kdTreeNode is NULL"
#define GET_KNN_FAIL_ERROR "An error occurred - could not retrieve KKN from the configuration file"
#define BPQ_EMPTY_ERROR "An error occurred - could not retrieve element, the BPQ is empty"

// info messages
#define IMAGE_PROC_INIT "Initializing imageProc"
#define IMAGE_PROC_SUCCESS "ImageProc was successfully initialized"
#define EXTRACTION_MODE_START "Starting extraction mode"
#define EXTRACTION_MODE_SUCCESS "Extraction mode finished successfully"
#define LOGGER_INIT_SUCCESS "Logger was successfully initialized"
#define KDTREE_INIT "Initializing KDTree"
#define KDTREE_SUCCESS "KDTree was successfully initialized"
#define EXTRACT_FEATURES_START "Extracting features from files"
#define EXTRACT_FEATURES_SUCCESS "Extraction completed successfully"

/*
 * Macros
 */

// free memory
#define FREE_FEATURES_ARRAY(featuresArray,numOfFeats) freeMainMemory(NULL,featuresArray,numOfFeats,false,NULL)
#define FREE_CONFIG(config) freeMainMemory(config,NULL,0,false,NULL)
#define FREE_ALL(config,featuresArray,numOfFeats,kdTree) delete imageProc;\
freeMainMemory(config,featuresArray,numOfFeats,true,kdTree);

// print and messages
//TODO fix print or delete it completely
#define PRINT(msg) printf("%s",msg);
#define LOGGER_INFO_EXTRACTION_MSG(buffer,msg,filename) sprintf(buffer,msg,filename);\
PRINT_INFO_LOGGER(buffer);
#define PRINT_ERROR_LOGGER(msg,file,func,line) if(spLoggerPrintError(msg,file,func,line)!= SP_LOGGER_SUCCESS){\
	PRINT(LOGGER_PRINT_FAILURE)\
	}
#define PRINT_WARNING_LOGGER(msg,file,func,line) if(spLoggerPrintWarning(msg,file,func,line)!= SP_LOGGER_SUCCESS){\
	PRINT(LOGGER_PRINT_FAILURE)\
	}
#define PRINT_INFO_LOGGER(msg) if(spLoggerPrintInfo(msg)!= SP_LOGGER_SUCCESS){\
	PRINT(LOGGER_PRINT_FAILURE)\
	}

#endif /* DEFINES_H_ */
