#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>
#include <stdlib.h>
void
xalloc_fail(void);

void *
xmalloc(size_t size);

void
xfree(void *memory);

#endif /* __MEMORY_H__ */
