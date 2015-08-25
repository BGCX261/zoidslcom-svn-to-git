#ifndef __QUEUE_H
#define __QUEUE_H

#include "utypes.h"

#define QUEUE_MAX_SIZE 10

/** @defgroup queue Queue
 * @{
 *
 * Queue related functions
 */

/** Queue definition
 */
typedef struct {
    Byte buf[QUEUE_MAX_SIZE];
    int in;
    int out;
    int cnt;
} Queue;

/** Initialize the queue
*/
void queueInit(Queue *q);

/** Put char 'c' in the queue pointed to by 'q'
* Returns false if operation failed (the queue is full)
*/
Bool queuePut(Queue *q, Byte c);

/** Get next element from queue
*  Returns -1 (an int) if there are no elements in the queue
*/
int queueGet(Queue *q);

//clear all elements
void queueClear(Queue *q);

/** Returns true if the queue is empty
*/
Bool queueEmpty(Queue *q);

/** Returns true if the queue if full
*/
Bool queueFull(Queue *q);

#endif /* __QUEUE_H */
