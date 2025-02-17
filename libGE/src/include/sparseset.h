#ifndef GAMEENGINE_SPARSESET_H
# define GAMEENGINE_SPARSESET_H

# include "vars.h"

typedef struct SparseSet {
	u32		*dense;
	u32		*sparse;
	void	**comp;
	size_t	compSize;
	size_t	chunkSize;
	u32		count;
	void	(*freeComp)(void *);
	void	*(*defaultCreator)(void);
}	SparseSet;

Bool	CreateSparseSet(SparseSet *ss, size_t compSize, size_t chunkSize, void *(*defaultCreator)(void), void (*freeComp)(void *));
Bool	ResizeSparseSet(SparseSet *ss);
Bool	AddToSparseSet(SparseSet *ss, void *comp, u32 id);
void	RemoveFromSparseSet(SparseSet *ss, u32 id);
void	DestroySparseSet(void *ss);

#endif