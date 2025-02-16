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
}	SparseSet;

SparseSet	*CreateSparseSet(size_t compSize, size_t chunkSize);
Bool		ResizeSparseSet(SparseSet *ss);
Bool		AddToSparseSet(SparseSet *ss, void *comp, u32 id);
void		RemoveFromSparseSet(SparseSet *ss, u32 id);
void		DestroySparseSet(SparseSet *ss);

#endif