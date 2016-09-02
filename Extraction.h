
#ifndef EXTRACTION_H_
#define EXTRACTION_H_


bool ExportFeats(const char* path, SPPoint* feats, int numOfFeats);

SPPoint* ImportFeats(const char* path, int* numOfFeats);

#endif /* EXTRACTION_H_ */
