#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
/**
 * SP Bounded Priority Queue summary
 *
 * TODO Complete documentation
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t* SPBPQueue;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Creates a new Bounded priority queue with bounded size.
 *
 * @param maxSize - The maximal number of elements allowed in the queue.
 * @return
 * NULL in case of memory allocation fails or if maxSize < 0.
 * Otherwise a new empty queue with size bound of maxSize.
 */
SPBPQueue spBPQueueCreate(int maxSize);

/**
 * Creates a copy of target bounded priority queue.
 *
 * The new copy will contain the same elements and size bound
 * as the original queue.
 *
 * @param source - The source queue which will be copied.
 * @return
 * NULL if a NULL was sent or memory allocation failed.
 * Otherwise a new copy of the source queue.
 **/
SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * Destroys a bounded priority queue.
 * All memory allocation associated with the queue and his elements will be freed.
 *
 * @param source - the queue which will be freed.
 * if source is NULL, then nothing is done.
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * Removes all elements from target bounded priority queue.
 *
 * The elements of the queue are deallocated using the stored freeing function.
 * @param source - the queue to remove all element from.
 * if source is NULL, then nothing is done.
 */
void spBPQueueClear(SPBPQueue source);

/**
 * Returns the number of elements in a bounded priority queue.
 *
 * @param source - The queue whose number of elements requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the queue.
 */
int spBPQueueSize(SPBPQueue source);

/**
 * Returns the maximal number of elements allowed in a bounded priority queue.
 *
 * @param source - The queue whose maximal size requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the queue.
 */
int spBPQueueGetMaxSize(SPBPQueue source);

/**
 * Adds a new element to the bounded priority queue.
 *
 * The element is inserted into the queue based on his priority, lower priority means
 * lower position in the queue.
 * If the queue is full, insert the new element and remove the highest priority element from the queue.
 *
 * The priority is decided based on the following relation:
 *
 * Two elements e1 and e2 are said to have same priority iff:
 * 		(e1.index == e2.index) AND (e1.value == e2.value)
 * Element e1 is lower priority than element e2 iff:
 * 		(e1.value < e2.value)   OR (e1.value == e2.value AND e1.index < e2.index)
 * Element e1 is higher priority than element e2 iff:
 * 		(e2 is less than e1)
 *
 * @param source - The queue into which the element is inserted.
 * @param element - The element to insert. A copy of the element will be inserted.
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT if a NULL was sent as source or a NULL was sent as element.
 * SP_BPQUEUE_OUT_OF_MEMORY if an allocation failed.
 * SP_BPQUEUE_FULL if the element has been inserted successfully and the queue was full, so the highest
 * priority element got removed from the queue.
 * SP_BPQUEUE_SUCCESS the element has been inserted successfully.
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * Removes the currently lowest priority element in the queue.
 *
 * @param source - The queue for which the lowest priority element will be removed.
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT if source is NULL.
 * SP_BPQUEUE_EMPTY if source is an empty queue.
 * SP_BPQUEUE_SUCCESS the element was removed successfully.
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * Returns a copy of the currently lowest priority element in the queue.
 *
 * @param source - The queue for which the lowest priority element will be returned.
 * @return
 * NULL if source is NULL or the queue is empty.
 * Otherwise a new copy of the lowest priority element in the queue.
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * Returns a copy of the currently highest priority element in the queue.
 *
 * @param source - The queue for which the highest priority element will be returned.
 * @return
 * NULL if source is NULL or the queue is empty.
 * Otherwise a new copy of the highest priority element in the queue.
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * Returns the minimal value of an element in the queue.
 *
 * @param source - The queue for which the minimal value will be returned.
 * @return
 * -1.0 if source is NULL or the queue is empty.
 * Otherwise the minimal value of an element in the queue.
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * Returns the maximal value of an element in the queue.
 *
 * @param source - The queue for which the maximal value will be returned.
 * @return
 * -1.0 if source is NULL or the queue is empty.
 * Otherwise the maximal value of an element in the queue.
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * Returns True if the queue is empty.
 *
 * @param source - The queue for which the check is performed.
 * @assert source!=NULL.
 * @return
 * True if the queue is empty
 * False otherwise.
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * Returns True if the queue is full.
 *
 * @param source - The queue for which the check is performed.
 * @assert source!=NULL.
 * @return
 * True if the queue is full
 * False otherwise.
 */
bool spBPQueueIsFull(SPBPQueue source);

#endif
