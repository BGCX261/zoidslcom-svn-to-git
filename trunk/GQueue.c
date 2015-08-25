#include "GQueue.h"
/*
struct gqueue
{
	void *ptr;
	int in, out, count, size, ele_size;
};
*/

GQueue* newGQueue(int n_ele, int ele_size)
{
	GQueue* q = (GQueue*) malloc(sizeof(GQueue));
	if(!q) return NULL;
	
	q->ptr = malloc(n_ele * ele_size);
	if(!(q->ptr))
	{
		free(q);
		return NULL;
	}
	
	q->in = q->out = q->count = 0;
	q->size = n_ele; q->ele_size = ele_size;

	return q;
}


void deleteGQueue(GQueue* q)
{
	free(q->ptr);
	free(q);
}


Bool putGQueue(GQueue* q, void* ele)
{
	if (isFullGQueue(q)) return false;
	
	// memcpy(dest, src, n_bytes) - “memory copy”
	memcpy(q->ptr + q->in, ele, q->ele_size);
	
	q->count++;
	q->in += q->ele_size;
	if(q->in == q->size * q->ele_size) q->in = 0;
	
	return true;
}


void* peekGQueue(GQueue* q)
{
	if (isEmptyGQueue(q)) return NULL;
	return q->ptr + q->out;
}


void* getGQueue(GQueue* q)
{
	if (isEmptyGQueue(q)) return NULL;
	void* pnt_data = q->ptr + q->out;
	
	q->count--;
	q->out += q->ele_size;
	if(q->out == q->size * q->ele_size) q->out = 0;
		
	return pnt_data;
}


void clearGQueue(GQueue* q)
{
	q->in = q->out = q->count = 0;
}


Bool isEmptyGQueue(GQueue* q)
{
	return q->count == 0;
}


Bool isFullGQueue(GQueue* q)
{
	return q->count == q->size;
}
