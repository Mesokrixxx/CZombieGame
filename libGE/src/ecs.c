#include "GE.h"

extern Instance	*instance;

ECS		*CreateECS()
{
	ECS	*ecs;

	ecs = _malloc(sizeof(ECS));
	if (!ecs)
		return ((void)LOG("Failed to allocate for new ECS\n"), NULL);

	ecs->comps = _malloc(sizeof(SparseSet));
	if (!ecs->comps)
		return (_free(ecs), (void)LOG("Failed to allocate for the comps list\n"), NULL);

	if (!CreateSparseSet(ecs->comps, sizeof(SparseSet), COMPONENT_CHUNK_SIZE, NULL, DestroySparseSet))
		return (_free(ecs->comps), _free(ecs), (void)LOG("Failed to create sparseset\n"), NULL);

	ecs->entityCount = 0;
	return (ecs);
}

void	DestroyECS(ECS *ecs)
{
	if (!ecs)
		return ((void)LOG("Tried to free a NULL ECS\n"));

	DestroySparseSet(ecs->comps);
	_free(ecs->comps);
	_free(ecs);
}

Bool		RegisterComponent(u32 compID, size_t compSize, void *(*defaultCreator)(void), void (*defaultRemover)(void *))
{
	SparseSet	ss;

	if (!CreateSparseSet(&ss, compSize, ENTITY_CHUNK_SIZE, defaultCreator, defaultRemover))
		return  ((void)LOG("Failed to create sparseset for new comp\n"), false);

	if (!AddToSparseSet(instance->entities->comps, &ss, compID))
		return ((void)LOG("Failed to register new comp\n"), false);

	return (true);
}

u32	CreateEntity(u32 flags)
{
	SparseSet	*comps;
	u32			entityID;

	comps = instance->entities->comps;
	entityID = instance->entities->entityCount;
	if (!AddToSparseSet(comps->comp[comps->sparse[FLAGS_CMP]], &flags, entityID))
		return ((void)LOG("Failed to add flags of new entities in ECS\n"), 0);

	for (u32 i = 0; i < comps->count; i++)
	{
		u32 compID = comps->dense[i];

		if (flags & compID)
			if (!AddToSparseSet(comps->comp[comps->sparse[compID]], NULL, entityID))
				return ((void)LOG("Failed to add comp to sparse set when creating entity %d\n", entityID), 0);
	}

	instance->entities->entityCount++;
	return (entityID);
}

u32		GetFlags(u32 entityID)
{
	SparseSet	*comps;
	SparseSet	*ssOfFlags;

	comps = instance->entities->comps;
	ssOfFlags = comps->comp[comps->sparse[FLAGS_CMP]];
	return (*(u32 *)ssOfFlags->comp[ssOfFlags->sparse[entityID]]);
}

void	RemoveEntity(u32 entityID)
{
	SparseSet	*comps;
	u32			flags;

	comps = instance->entities->comps;
	flags = GetFlags(entityID);
	if (!flags)
		return ((void)LOG("Flags of entity wasn't found\n"));
	for (u32 i = 0; i < comps->count; i++)
	{
		u32 compID = *(u32 *)comps->comp[comps->dense[i]];

		if (flags & compID)
			RemoveFromSparseSet(comps->comp[comps->sparse[compID]], entityID);
	}
}
