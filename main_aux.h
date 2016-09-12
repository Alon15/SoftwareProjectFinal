#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

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
 * @param config - the configuration structure
 *
 * @return True if the logger was successfully initialized
 * 		   False if the logger failed to initialize (an error message will be displayed)
 */
bool initLogger(SPConfig config);

/*
 * Free the memory of the main function variables.
 *
 * Freeing featuresArray will also free the memory of all the points stored in the array
 * If a parameter is NULL, then it won't be freed
 *
 * @param config - the configuration structure to be freed
 * @param featuresArray - array of SPPoints
 * @param numOfFeats - the number of elements in the featuresArray
 * @param logger - a boolean that indicate if the logger will be freed
 *
 */
void freeMainMemory(SPConfig config,SPPoint* featuresArray,int numOfFeats, bool logger);

void tmpFunc1();

#endif /* MAIN_AUX_H_ */
