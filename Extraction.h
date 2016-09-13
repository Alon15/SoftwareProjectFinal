#ifndef EXTRACTION_H_
#define EXTRACTION_H_

#include "SPPoint.h"

/*
 * Export an array of SPPoint (features array) into file given by the path string.
 *
 * The function save the data (of type double) in a precision of up to 4 decimal digits
 *
 * @param path - the path to the file we want to export the SPPoint array to
 * @param feats - array of SPPoint which will be stored in the file
 * @param numOfFeats - the number of features in the 'feats' array
 *
 * @return true if the export completed successfully
 * 		   false if the export failed
 */
bool ExportFeats(const char* path, SPPoint* feats, int numOfFeats);

/*
 * Import an array of SPPoint (features array) from a file given by the path string.
 *
 * @param path - the path to the file we want to import the SPPoint array from
 * @param numOfFeats - a pointer to the number of features the imported array has
 *
 * The function prints a warning if one of the features is corrupted (bad format).
 * If more then CORRUPTED_FILE_ERROR_LIMIT (defined in defines.h) of the features are corrupted
 * the function will stop the import process and print an error (a NULL pointer will be returned)
 *
 * @return a pointer to the extracted SPPoint array if the import completed successfully
 * 		   a NULL pointer if the import failed
 */
SPPoint* ImportFeats(const char* path, int* numOfFeats);

#endif /* EXTRACTION_H_ */
