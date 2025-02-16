#include "GE.h"

SparseSet	*CreateSparseSet(size_t compSize, size_t chunkSize)
{
	SparseSet	*ss;

	ss = _malloc(sizeof(SparseSet));
	if (!ss)
	{
		LOG("Failed to create new SparseSet\n");
		return (NULL);
	}
	
	ss->comp = _malloc(chunkSize * compSize);
	if (!ss->comp)
	{
		LOG("Failed to create comp module of new SparseSet\n");
		return (_free(ss), NULL);
	}

	ss->dense = _malloc(chunkSize * sizeof(u32));
	if (!ss->dense)
	{
		LOG("Failed to create dense module of new SparseSet\n");
		return (_free(ss->comp), _free(ss), NULL);
	}

	ss->sparse = _malloc(chunkSize * sizeof(u32));
	if (!ss->sparse)
	{
		LOG("Failed to create sparse module of new SparseSet\n");
		return (_free(ss->dense), _free(ss->comp), _free(ss), NULL);
	}

	ss->chunkSize = chunkSize;
	ss->compSize = compSize;
	ss->comp = 0;
	return (ss);
}

Bool	ResizeSparseSet(SparseSet *ss)
{
	u32		*tempReallocDense;
	u32		*tempReallocSparse;
	void	**tempReallocComp;

	tempReallocDense = realloc(ss->dense, (ss->chunkSize + ss->count) * sizeof(u32));
	if (!tempReallocDense)
		return ((void)LOG("Failed to resize dense part of sparse set\n"), false);

	tempReallocSparse = realloc(ss->sparse, (ss->chunkSize + ss->count) * sizeof(u32));
	if (!tempReallocSparse)
		return ((void)LOG("Failed to resize sparse part of sparse set\n"), false);

	tempReallocComp = realloc(ss->comp, (ss->chunkSize + ss->count) * ss->compSize);
	if (!tempReallocComp)
		return ((void)LOG("Failed to resize comp part of sparse set\n"), false);

	ss->dense = tempReallocDense;
	ss->sparse = tempReallocSparse;
	ss->comp = tempReallocComp;

	return (true);
}

Bool	AddToSparseSet(SparseSet *ss, void *comp, u32 id)
{
	if (!ss)
		return ((void)LOG("Trying to add a comp to a NULL SparseSet\n"), false);
	if (!comp)
		return ((void)LOG("Trying to add a NULL comp to an SparseSet\n"), false);
	if (ss->count % ss->chunkSize == 0)
		if (!ResizeSparseSet(ss))
			return ((void)LOG("Failed to resize SparseSet when adding a new comp\n"), false);
	ss->comp[ss->count] = _malloc(ss->compSize);
	if (!ss->comp[ss->count])
		return ((void)LOG("Failed to allocate when adding a comp to an SparseSet\n"), false);
	memcpy(ss->comp, comp, ss->compSize);
	ss->dense[ss->count] = id;
	ss->sparse[id] = ss->count;
	ss->count++;
	return (true);
}

void	RemoveFromSparseSet(SparseSet *ss, u32 id)
{
	u32	entityIndex;
	u32	lastEntityID;
	
	if (!ss)
		return ((void)LOG("Trying to remove a comp to a NULL SparseSet\n"));
	if (id >= ss->count)
		return ((void)LOG("Trying to remove a non existing comp from a SparseSet\n"));
	entityIndex = ss->sparse[id];
	lastEntityID = ss->dense[ss->count - 1];

	ss->dense[entityIndex] = lastEntityID;
	ss->sparse[lastEntityID] = entityIndex;
	
	_free(ss->comp[entityIndex]);
	ss->comp[entityIndex] = ss->comp[ss->count - 1];
	ss->comp[ss->count - 1] = NULL;

	ss->count--;
}

void	DestroySparseSet(SparseSet *ss)
{
	if (!ss)
		return ((void)LOG("Trying to destroy a NULL SparseSet\n"));
	for (u32 i = 0; i < ss->count; i++)
		_free(ss->comp[i]);
	_free(ss->comp);
	_free(ss->dense);
	_free(ss->sparse);
	_free(ss);
}
