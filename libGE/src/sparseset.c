#include "GE.h"

Bool	CreateSparseSet(SparseSet *ss, size_t compSize, size_t chunkSize, void *(*defaultCreator)(void), void (*freeComp)(void *))
{	
	ss->comp = _malloc(chunkSize * compSize);
	if (!ss->comp)
	{
		LOG("Failed to create comp module of new SparseSet\n");
		return (_free(ss), false);
	}
	memset(ss->comp, 0, chunkSize * compSize);

	ss->dense = _malloc(chunkSize * sizeof(u32));
	if (!ss->dense)
	{
		LOG("Failed to create dense module of new SparseSet\n");
		return (_free(ss->comp), _free(ss), false);
	}

	ss->sparse = _malloc(chunkSize * sizeof(u32));
	if (!ss->sparse)
	{
		LOG("Failed to create sparse module of new SparseSet\n");
		return (_free(ss->dense), _free(ss->comp), _free(ss), false);
	}

	ss->chunkSize = chunkSize;
	ss->compSize = compSize;
	ss->freeComp = freeComp;
	ss->defaultCreator = defaultCreator;
	ss->count = 0;
	return (true);
}

Bool	ResizeSparseSet(SparseSet *ss)
{
	u32		*tempReallocDense;
	u32		*tempReallocSparse;
	void	**tempReallocComp;

	LOG("SS Resizing\n");

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

	LOG("SS Resized\n");

	return (true);
}

Bool	AddToSparseSet(SparseSet *ss, void *comp, u32 id)
{	
	if (!ss)
		return ((void)LOG("Trying to add a comp to a NULL SparseSet\n"), false);
	if (!comp)
	{
		if (!ss->defaultCreator)
			return ((void)LOG("Trying to add NULL comp with a non defined default creator\n"), false);
		comp = ss->defaultCreator();
	}	
	if (ss->count % ss->chunkSize == 0 && ss->count > 0)
		if (!ResizeSparseSet(ss))
			return ((void)LOG("Failed to resize SparseSet when adding a new comp\n"), false);
	ss->comp[ss->count] = _malloc(ss->compSize);
	if (!ss->comp[ss->count])
		return ((void)LOG("Failed to allocate when adding a comp to an SparseSet\n"), false);
	memcpy(ss->comp[ss->count], comp, ss->compSize);
	
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
	
	if (ss->freeComp)
		ss->freeComp(ss->comp[entityIndex]);
	_free(ss->comp[entityIndex]);
	ss->comp[entityIndex] = ss->comp[ss->count - 1];
	ss->comp[ss->count - 1] = NULL;

	ss->count--;
}

void	DestroySparseSet(void *ss)
{
	if (!ss)
		return ((void)LOG("Trying to destroy a NULL SparseSet\n"));
	
	SparseSet	*SS = ss;

	for (u32 i = 0; i < SS->count; i++)
	{
		if (SS->freeComp)
			SS->freeComp(SS->comp[i]);
		_free(SS->comp[i]);
	}
	_free(SS->comp);
	_free(SS->dense);
	_free(SS->sparse);
}
