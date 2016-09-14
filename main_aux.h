#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

#include "SPConfig.h"
#include "KDTreeNode.h"

/*
 * The function gets the configuration filename from the command line arguments or sets it to
 * the default file name if no arguments were given.
 *
 * Any arguments that are not in the format: "-c <config_filename>" will cause an error.
 *
 * @param filename - pointer in which the file name returned by the function is stored
 * @param argc - number of arguments
 * @param argv - array of arguments (each argument is a string)
 *
 */
void getFileName(char* filename, int argc, char** argv);

/*
 * Initialize the logger.
 *
 * @param config - The configuration structure
 *
 * @return True if the logger was successfully initialized
 * 		   False if the initialization failed (an error message will be displayed)
 */
bool initLogger(SPConfig config);

/*
 * Free the memory of the main function variables.
 *
 * Freeing featuresArray will also free the memory of all the points stored in the array
 * If a parameter is NULL, then it won't be freed
 *
 * @param config - The configuration structure to be freed
 * @param featuresArray - Array of SPPoints
 * @param numOfFeats - The number of elements in the featuresArray
 * @param logger - A boolean that indicate if the logger will be freed
 *
 */
void freeMainMemory(SPConfig config,SPPoint* featuresArray,int numOfFeats, bool logger,KDTreeNode kdTree);

/*
 * Extract all the features from the ".feats" files of all the images in the directory, and return
 * a pointer to the features extracted, also store the number of features extracted in 'numOfFeats'
 *
 *  @param config - The configuration structure
 *  @param numOfFeats - A pointer to a integer that will store the number of features extracted
 *
 *  @return Pointer to the features array if the extraction finished successfully
 *  		NULL if extraction failed (an error message will be displayed)
 */
SPPoint* extractAllFeatures(SPConfig config, int* numOfFeats);

/*
 * Checks if the file exists and if it is readable
 *
 * @param fileName - Path to the file
 *
 * @return True if the file exists and readable
 * 		   False otherwise
 */
bool fileCheck(const char* fileName);

#endif /* MAIN_AUX_H_ */
