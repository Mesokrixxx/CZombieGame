#include "GE.h"

#ifdef DEBUG

int	allocCount = 0;

void	*malloc_debug(size_t size)
{
	allocCount++;
	LOG("Allocating %zu bytes, current allocation count: %d\n", size, allocCount);
	return (malloc(size));
}

void	free_debug(void *ptr)
{
	allocCount--;
	LOG("Freeing %p, current allocation count: %d\n", ptr, allocCount);
	return (free(ptr));
}

#endif
