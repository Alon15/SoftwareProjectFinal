#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

//#include "KDTreeNode.h" // TODO Can we delete this line?
#include "SPConfig.h"

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
void freeMainMemory(SPConfig config,SPPoint* featuresArray,int numOfFeats, bool logger);

/* TODO
 * TODO
 */
bool extractAllFeatures(SPConfig config, SPPoint* featuresArray, int* numOfFeats);

/*
 * Checks if the file exists and if it is readable
 *
 * @param fileName - Path to the file
 *
 * @return True if the file exists and readable
 * 		   False otherwise
 */
bool fileCheck(const char* fileName);

void tmpFunc1(); // TODO DEBUG DELME

#endif /* MAIN_AUX_H_ */
