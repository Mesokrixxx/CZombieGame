#ifndef GAME_ENGINE_SPARSESET_H
# define GAME_ENGINE_SPARSESET_H

# include "GEVars.h"

typedef struct {
	void	**comp;
	u32		*dense;
	u32		*sparse;
	u32		count;
	size_t	compSize;
	size_t	chunkSize;
	void	*(*defaultCreator)(void);
	void	(*freeComp)(void *);
}	GESparseSet;

bool	GECreateSparseSet(GESparseSet *ss, size_t compSize, size_t chunkSize, void *(*defaultCreator)(void), void (*freeComp)(void *));
bool	GEAddToSparseSet(GESparseSet *ss, void *comp, u32 id);
void	GERemoveFromSparseSet(GESparseSet *ss, u32 id);
void	*GEGetFromSparseSet(GESparseSet *ss, u32 id);
void	GEDestroySparseSet(void *sparseset);

#endif