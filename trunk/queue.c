#include "queue.h"

void queueInit(Queue* q)
{
	q->in = q->out = q->cnt = 0;
}


Bool queuePut(Queue* q, Byte elem)
{
	if(q->cnt == QUEUE_MAX_SIZE) return false;
	(q->buf)[q->in] = elem;
	
	q->cnt++; q->in++;
	if(q->in == QUEUE_MAX_SIZE) q->in = 0;
	return true;
}


int queueGet(Queue* q)
{
	if(q->cnt == 0) return -1;
	Byte data = (q->buf)[q->out];
	
	q->cnt--; q->out++;
	if(q->out == QUEUE_MAX_SIZE) q->out = 0;
	return data;
}


Bool queueEmpty(Queue* q)
{
	return (q->cnt == 0);
}


Bool queueFull(Queue* q)
{
	return (q->cnt == QUEUE_MAX_SIZE);
}
