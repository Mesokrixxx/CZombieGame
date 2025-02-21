#include "GEPrivate.h"

size_t	allocCount = 0;
size_t	allocSize = 0;

void	*GEPMalloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	allocCount++;
	allocSize += size;
	return (ptr);
}

void	*GEPRealloc(size_t isize, size_t size, void *ptr)
{
	void	*out;

	out = realloc(ptr, size);
	if (!out)
		return (NULL);
	allocSize += (size - isize);
	return (out);
}

void	GEPFree(void *ptr)
{
	free(ptr);
	allocCount--;
}

void	GEPAllocStats()
{
	printf("Still %zu allocations to free.\n", allocCount);
	printf("Allocated %zu bytes:\n - %zu KB\n - %zu MB\n",
		allocSize, allocSize / 1024, allocSize / 1024 / 1024);
}
