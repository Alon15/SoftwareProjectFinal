#include "SPBPriorityQueue.h"
#include "SPList.h"
#include "SPListElement.h"
#include <stdlib.h> // malloc, free, NULL
#include <assert.h> // assert

struct sp_bp_queue_t {
	SPList elementList;
	int maxSize;
};

SPBPQueue spBPQueueCreate(int maxSize) {
	// Function variables
	SPBPQueue BPQueue;
	SPList elemList;
	// Function code
	if (maxSize < 0) {
		return NULL; // Invalid parameters
	}
	BPQueue = (SPBPQueue) malloc(sizeof(struct sp_bp_queue_t));
	if (BPQueue == NULL) { // Allocation Fails
		return NULL;
	}
	elemList = spListCreate();
	if (elemList == NULL) { // Allocation Fails
		free(BPQueue);
		return NULL;
	}
	BPQueue->maxSize = maxSize;
	BPQueue->elementList = elemList;
	return BPQueue;
}

SPBPQueue spBPQueueCopy(SPBPQueue source) {
	// Function variables
	SPBPQueue newBPQueue;
	// Function code
	if (source == NULL) {
		return NULL; // Invalid source queue
	}
	newBPQueue = spBPQueueCreate(source->maxSize);
	if (newBPQueue != NULL) { // Creation Succeeded
		spListDestroy(newBPQueue->elementList); // Free the empty list
		newBPQueue->elementList = spListCopy(source->elementList); // Copy source's list
	}
	return newBPQueue;
}

void spBPQueueDestroy(SPBPQueue source) {
	if (source != NULL) {
		spListDestroy(source->elementList);
		free(source);
	}
}

void spBPQueueClear(SPBPQueue source) {
	if (source != NULL) {
		spListClear(source->elementList);
	}
}

int spBPQueueSize(SPBPQueue source) {
	if (source == NULL) {
		return -1;
	} else {
		return spListGetSize(source->elementList);
	}
}

int spBPQueueGetMaxSize(SPBPQueue source) {
	if (source == NULL) {
		return -1;
	} else {
		return source->maxSize;
	}
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element) {
	// Function variables
	SPListElement iter;
	SP_LIST_MSG listIndicator;
	// Function code
	if (source == NULL || element == NULL) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	iter = spListGetFirst(source->elementList);
	if (iter == NULL) { // The list is empty
		listIndicator = spListInsertFirst(source->elementList,element);
		if (listIndicator == SP_LIST_OUT_OF_MEMORY) { // Allocation Fails
			return SP_BPQUEUE_OUT_OF_MEMORY;
		}
		return SP_BPQUEUE_SUCCESS;
	}
	while (iter != NULL && spListElementCompare(iter,element) < 0) { // Find insertion point
		iter = spListGetNext(source->elementList);
	}
	// Insert a copy of the element (allocation inside spListInsert)
	if (iter == NULL) { // insert to the end of the queue
		listIndicator = spListInsertLast(source->elementList,element);
	} else {
		listIndicator = spListInsertBeforeCurrent(source->elementList,element);
	}
	if (listIndicator == SP_LIST_OUT_OF_MEMORY) { //Allocation Fails
		return SP_BPQUEUE_OUT_OF_MEMORY;
	}
	if (spBPQueueSize(source) > spBPQueueGetMaxSize(source)) { // Queue overflow
		spListGetLast(source->elementList); // Move the list pointer to the last element
		spListRemoveCurrent(source->elementList); // Remove the last element
		return SP_BPQUEUE_FULL;
	}
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source) {
	if (source == NULL) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if (spBPQueueIsEmpty(source)) {
		return SP_BPQUEUE_EMPTY;
	}
	spListGetFirst(source->elementList);
	spListRemoveCurrent(source->elementList);
	return SP_BPQUEUE_SUCCESS;
}

SPListElement spBPQueuePeek(SPBPQueue source) {
	if (source == NULL || spBPQueueIsEmpty(source)) {
		return NULL;
	} else {
		return spListElementCopy(spListGetFirst(source->elementList));
	}
}

SPListElement spBPQueuePeekLast(SPBPQueue source) {
	if (source == NULL || spBPQueueIsEmpty(source)) {
		return NULL;
	} else {
		return spListElementCopy(spListGetLast(source->elementList));
	}
}

double spBPQueueMinValue(SPBPQueue source) {
	if (source == NULL || spBPQueueIsEmpty(source)) {
		return -1.0;
	} else {
		return spListElementGetValue(spListGetFirst(source->elementList));
	}
}

double spBPQueueMaxValue(SPBPQueue source) {
	if (source == NULL || spBPQueueIsEmpty(source)) {
		return -1.0;
	} else {
		return spListElementGetValue(spListGetLast(source->elementList));
	}
}

bool spBPQueueIsEmpty(SPBPQueue source) {
	assert(source != NULL);
	if (spBPQueueSize(source) == 0) {
		return true;
	} else {
		return false;
	}
}

bool spBPQueueIsFull(SPBPQueue source) {
	assert(source != NULL);
	if (spBPQueueSize(source) == source->maxSize) {
		return true;
	} else {
		return false;
	}
}
