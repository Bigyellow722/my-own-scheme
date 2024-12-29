#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stddef.h>

void *xmalloc(size_t size);
void *xrealloc(void* ptr, size_t size);
void xfree(void *ptr);
void *xmemmove(void *dest, const void *src, size_t n);
void* xmemset(void *s, int c, size_t n);

#endif
