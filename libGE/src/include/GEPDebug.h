#ifndef GAME_ENGINE_PRIVATE_DEBUG_H
# define GAME_ENGINE_PRIVATE_DEBUG_H

# include "GEVars.h"

# define _malloc(size)				GEPMalloc(size)
# define _realloc(isize, size, ptr)	GEPRealloc(isize, size, ptr);
# define _free(ptr)					GEPFree(ptr)

void	*GEPMalloc(size_t size);
void	*GEPRealloc(size_t isize, size_t size, void *ptr);
void	GEPFree(void *ptr);
void	GEPAllocStats();

#endif