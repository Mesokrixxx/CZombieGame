#include "GE.h"

extern Instance	*instance;

ECS		*CreateECS()
{
	ECS	*ecs;

	ecs = _malloc(sizeof(ECS));
	if (!ecs)
		return ((void)LOG("Failed to allocate for new ECS\n"), NULL);

	ecs->comps = CreateSparseSet(sizeof(SparseSet), COMPONENT_CHUNK_SIZE, freeComponentInECS);
	if (!ecs->comps)
		return (_free(ecs->comps), (void)LOG("Failed to allocate for the comps list\n"), NULL);

	ecs->entityCount = 0;
	return (ecs);
}

void	DestroyECS(ECS *ecs)
{
	if (!ecs)
		return ((void)LOG("Tried to free a NULL ECS\n"));

	DestroySparseSet(ecs->comps);
	_free(ecs);
}

void	freeComponentInECS(void *data)
{
	SparseSet	*ss;

	if (!data)
		return ((void)LOG("Failed to free NULL comp in ECS\n"));
	ss = data;
	for (u32 i = 0; i < ss->count; i++)
	{
		if (ss->freeComp)
			ss->freeComp(ss->comp[i]);
		_free(ss->comp[i]);
	}
	_free(ss->comp);
	_free(ss->sparse);
	_free(ss->dense);
}
