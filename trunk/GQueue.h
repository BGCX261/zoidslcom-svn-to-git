#ifndef GQUEUE_H_
#define GQUEUE_H_

#include <stdlib.h>
#include <string.h>

#include "utypes.h"

typedef struct
{
	void *ptr;
	int in, out, count, size, ele_size;
	
} GQueue;

GQueue* newGQueue(int n_ele, int ele_size);
void deleteGQueue(GQueue* q);
Bool putGQueue(GQueue* q, void* ele);
void* peekGQueue(GQueue* q);
void* getGQueue(GQueue* q);
void clearGQueue(GQueue* q);
Bool isEmptyGQueue(GQueue* q);
Bool isFullGQueue(GQueue* q);

#endif
