#ifndef EXTRACTION_H_
#define EXTRACTION_H_
#include "SPPoint.h"
/*
 * @@@@@@@@@@@
 *
 * @@@@@@@@@@@
 */
bool ExportFeats(const char* path, SPPoint* feats, int numOfFeats);

/*
 * @@@@@@@@@@@ TODO
 *
 * @@@@@@@@@@@ TODO
 */
SPPoint* ImportFeats(const char* path, int* numOfFeats);

#endif /* EXTRACTION_H_ */
