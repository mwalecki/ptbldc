#ifndef _CIRCBUF_H
#define _CIRCBUF_H

#include <inttypes.h>
#include <stdio.h>

/* Opaque buffer element type.  This would be defined by the application. */
typedef	uint8_t ElemType;
 
/* Circular buffer object */
typedef struct 
{
    uint16_t	size;   /* maximum number of elements           */
    uint16_t	start;  /* index of oldest element              */
    uint16_t	count;    /* index at which to write new element  */
    ElemType	*elems;  /* vector of elements                   */
} CircularBuffer;

void cbInit(CircularBuffer *cb, ElemType *elemArray, int size);
int cbIsFull(CircularBuffer *cb);
int cbIsEmpty(CircularBuffer *cb);
/* Write an element, overwriting oldest element if buffer is full. App can
   choose to avoid the overwrite by checking cbIsFull(). */
void cbWrite(CircularBuffer *cb, ElemType *elem);
/* Read oldest element. App must ensure !cbIsEmpty() first. */
void cbRead(CircularBuffer *cb, ElemType *elem);
int cbLength(CircularBuffer *cb);

#endif // _CIRCBUF_H
