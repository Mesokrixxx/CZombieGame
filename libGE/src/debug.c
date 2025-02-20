#include "GE.h"

u32		allocCount = 0;
size_t	allocSize = 0;

void	*malloc_debug(size_t size)
{
	void	*ptr = malloc(size);
	if (!ptr)
		return (NULL);
	allocCount++;
	allocSize += size;
	return (ptr);
}

void	free_debug(void *ptr)
{
	allocCount--;
	free(ptr);
}

void	printAllocStats(void)
{
	printf("Still %d allocations to free\n", allocCount);
	printf("Allocated %zu bytes:\n - %zu KB\n - %zu MB\n", allocSize, allocSize / 1024, allocSize / 1024 / 1024);
}
