#include "GEPrivate.h"

static bool	_resizeSS(GESparseSet *ss);

bool	GECreateSparseSet(GESparseSet *ss, size_t compSize, size_t chunkSize, void *(*defaultCreator)(void), void (*freeComp)(void *))
{
	ASSERT(ss,
		"Trying to create a sparse set but given is NULL\n");

	ss->compSize = compSize;
	ss->chunkSize = chunkSize;
	ss->freeComp = freeComp ? freeComp : GEPFree;
	ss->defaultCreator = defaultCreator;
	ss->count = 0;

	ss->dense = _malloc(sizeof(u32) * chunkSize);
	if (!ss->dense)
		return (false);
	memset(ss->dense, UINT32_MAX, chunkSize * sizeof(u32));

	ss->sparse = _malloc(sizeof(u32) * chunkSize);
	if (!ss->sparse)
		return (_free(ss->dense), false);
	memset(ss->sparse, UINT32_MAX, chunkSize * sizeof(u32));

	ss->comp = _malloc(chunkSize * compSize);
	if (!ss->comp)
		return (_free(ss->dense), _free(ss->sparse), false);
	memset(ss->comp, 0, chunkSize * compSize);
	
	return (true);
}

bool	GEAddToSparseSet(GESparseSet *ss, void *comp, u32 id)
{
	ASSERT(ss,
		"Trying to add to a NULL sparse set\n");

	ASSERT(comp || ss->defaultCreator,
		"Trying to add a NULL comp with a NULL defaultCreator to a sparse set\n")

	if(ss->sparse[id] != UINT32_MAX)
		return (true); // Content already exist, we can just override it

	if (ss->count > 0 && ss->count % ss->chunkSize == 0)
	{
		if (!_resizeSS(ss))
		{
			printf("Failed to resize sparse set\n");
			return (false);
		}
	}

	comp = comp ? comp : ss->defaultCreator();

	ss->comp[ss->count] = _malloc(ss->compSize);
	if (!ss->comp[ss->count])
		return (false);
	memcpy(ss->comp[ss->count], comp, ss->compSize);
	
	ss->dense[ss->count] = id;
	ss->sparse[id] = ss->count;
	ss->count++;
	return (true);
}

void	GERemoveFromSparseSet(GESparseSet *ss, u32 id)
{
	u32	index;
	u32	lastIndex;
	u32	movedID;

	ASSERT(ss,
		"Trying to remove from a NULL sparse set\n");

	ASSERT(ss->comp[ss->sparse[id]],
		"Can't remove a non set component\n");

	index = ss->sparse[id];
	lastIndex = ss->count - 1;

	ss->freeComp(ss->comp[index]);
	ss->comp[index] = NULL;
	
	if (index != lastIndex)
	{
		movedID = ss->dense[lastIndex];

		ss->comp[index] = ss->comp[lastIndex];
		ss->comp[lastIndex] = NULL;

		ss->dense[index] = movedID;
		ss->sparse[movedID] = index;
	}

	ss->dense[lastIndex] = ss->count + (ss->chunkSize - 1);
	ss->sparse[id] = ss->count + (ss->chunkSize - 1);

	ss->count--;
}

void	*GEGetFromSparseSet(GESparseSet *ss, u32 id)
{
	ASSERT(ss,
		"Trying to get component from NULL sparse set\n");

	if (ss->sparse[id] == UINT32_MAX)
		return (NULL);

	return (ss->comp[ss->sparse[id]]);
}

void	GEDestroySparseSet(void *sparseset)
{
	GESparseSet	*ss;

	ss = sparseset;
	ASSERT(ss,
		"Trying to destroy a NULL sparse set\n");

	for (u32 i = 0; i < ss->count; i++)
		ss->freeComp(ss->comp[i]);
	_free(ss->comp);
	_free(ss->dense);
	_free(ss->sparse);
}

static bool	_resizeSS(GESparseSet *ss)
{
	u32		*tempDenseRealloc;
	u32		*tempSparseRealloc;
	void	**tempCompRealloc;

	tempDenseRealloc = _realloc(
		ss->count * sizeof(u32), 
		(ss->count + ss->chunkSize) * sizeof(u32), ss->dense);
	if (!tempDenseRealloc)
		return (false);

	tempSparseRealloc = _realloc(
		ss->count * sizeof(u32),
		(ss->count + ss->chunkSize) * sizeof(u32), ss->sparse);
	if (!tempSparseRealloc)
		return (false);

	tempCompRealloc = _realloc(
		ss->count * ss->compSize,
		(ss->count + ss->chunkSize) * ss->compSize, ss->comp);
	if (!tempCompRealloc)
		return (false);

	ss->dense = tempDenseRealloc;
	ss->sparse = tempSparseRealloc;
	ss->comp = tempCompRealloc;
	return (true);
}
