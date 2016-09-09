#ifndef DEFINES_H_
#define DEFINES_H_

// general
#define STRING_LENGTH 1025 // 1024 + \0
#define BUFFER_SIZE 1024 // Size of the file read/write buffer

// main
#define QUERY_IMG_MSG "Please enter an image path:\n"
#define EXIT_MSG "Exiting...\n"

// Extraction
#define MAX_FEATURE_DIM 28
#define CORRUPTED_FILE_ERROR_LIMIT 10

// SPConfig
#define NON_DEFAULT_CONFIG_PARAM 4 // Number of non default parameters in the config file
#define CONFIG_PARAM 14 // Number of parameters in the config file
/*
 * messages
 */

// main messages
#define GET_IMAGE_PATH_FAIL_ERROR "An error occurred - could not retrieve image path from the configuration file"
#define GET_FEATS_PATH_FAIL_ERROR "An error occurred - could not retrieve feats path from the configuration file"
#define FEATURES_EXTRACTION_FROM_IMAGE_FAIL_ERROR "An error occurred - could not extract features from an image"
// Extraction messages
#define WRITE_FILE_ERROR "An error occurred - could not write to file"
#define PATH_IS_NULL_ERROR "An error occurred - invalid path to file"
#define OPEN_FILE_FAIL_ERROR "An error occurred - could not open file"
#define STR_TO_INT_FAIL_WARNING "Feature corrupted - could not parse expected integer"
#define STR_TO_DOUBLE_FAIL_WARNING "Feature corrupted - could not parse expected double"
#define CORRUPTED_DATA_DIM_WARNING "Feature corrupted - Dimension and actual number coordinates does not match"
#define CORRUPTED_FILE_ERROR "An error occurred - could not extract features from file, big part of the data is invalid"
#define CORRUPTED_FILE_TOO_MANY_FEATURES "An error occurred - number of features parameter does not match the the actual features extracted"
#define MEMORY_ALLOCATION_ERROR "An error occurred - allocation failure"
// main_aux messages
#define GET_LOGGER_FILENAME_FAIL_ERROR "An error occurred - could not retrieve logger's filename from the configuration file"
#define LOGGER_LEVEL_INVALID_ERROR "An error occurred - logger level is invalid"
#define LOGGER_CREATE_FAIL_ERROR "An error occurred - could not initialize logger to file"
#define DEFAULT_LOGGER_CREATE_FAIL_ERROR "An error occurred - could not initialize logger to stdout"

#endif /* DEFINES_H_ */
