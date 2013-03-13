#include "circbuf.h"

void cbInit(CircularBuffer *cb, ElemType *elemArray, int size) {
		cb->elems = elemArray;
    cb->size  = size;
    cb->start = 0;
    cb->count = 0;
}
 
int cbIsFull(CircularBuffer *cb) {
    return cb->count == cb->size; }
 
int cbIsEmpty(CircularBuffer *cb) {
    return cb->count == 0; }
 
void cbWrite(CircularBuffer *cb, ElemType *elem) {
    int end = (cb->start + cb->count) % cb->size;
    cb->elems[end] = *elem;
    if (cb->count == cb->size)
        cb->start = (cb->start + 1) % cb->size; /* full, overwrite */
    else
        ++ cb->count;
}
 
void cbRead(CircularBuffer *cb, ElemType *elem) {
    *elem = cb->elems[cb->start];
    cb->start = (cb->start + 1) % cb->size;
		if(cb->count > 0)
			-- cb->count;
}
 
int cbLength(CircularBuffer *cb) {
    return cb->count;
}
