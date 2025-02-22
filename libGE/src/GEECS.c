#include "GEPrivate.h"

static void	_clearSS(void *ss) { GEDestroySparseSet(ss); _free(ss); }

bool	GECreateECS(GEECS *ecs)
{
	ASSERT(ecs,
		"Trying to create an ECS but given one is NULL\n");

	ecs->entities = _malloc(sizeof(GESparseSet));
	if (!ecs->entities)
		return (false);
	if (!GECreateSparseSet(ecs->entities, sizeof(u32), GE_ENTITY_CHUNK_SIZE, NULL, NULL))
		return (_free(ecs->entities), false);

	ecs->systems = _malloc(sizeof(GESparseSet));
	if (!ecs->systems)
		return (_clearSS(ecs->entities), false);
	if (!GECreateSparseSet(ecs->systems, sizeof(GESystem), GE_SYSTEM_CHUNK_SIZE, NULL, NULL))
		return (_clearSS(ecs->entities), _free(ecs->systems), false);

	ecs->components = _malloc(sizeof(GESparseSet));
	if (!ecs->components)
		return (_clearSS(ecs->systems), _clearSS(ecs->entities), false);
	if (!GECreateSparseSet(ecs->components, sizeof(GESparseSet), GE_COMPS_CHUNK_SIZE, NULL, _clearSS))
		return (_clearSS(ecs->entities), _clearSS(ecs->systems), _free(ecs->components), false);

	return (true);
}

bool	GERegisterComponent(GEECS *ecs, GEComponent comp)
{
	GESparseSet	ss;

	ASSERT(ecs,
		"Trying to register a comp to a NULL ecs\n");

	ASSERT(!GEGetFromSparseSet(ecs->components, comp.compID),
		"Trying to register a comp but comp's flag already exist\n");

	if (!GECreateSparseSet(&ss, comp.compSize, GE_ENTITY_CHUNK_SIZE, comp.defaultCreator, comp.defaultRemover))
		return (false);

	return (GEAddToSparseSet(ecs->components, &ss, comp.compID));
}

bool	GERegisterSystem(GEECS *ecs, GESystem system)
{
	ASSERT(ecs,
		"Trying to register a system to a NULL ecs\n");

	ASSERT(system.action,
		"Cannot register a system with NULL action\n");
	
	return (GEAddToSparseSet(ecs->systems, &system, ecs->systems->count));
}

u32	GECreateEntity(GEECS *ecs, u32 flags)
{
	u32	entityID;

	ASSERT(ecs,
		"Trying to create entity with NULL ecs\n");

	entityID = ecs->entities->count;

	for (u32 i = 0; i < ecs->components->count; i++)
	{
		u32	compID = ecs->components->dense[i];

		if (flags & compID)
			if (!GEAddToSparseSet(ecs->components->comp[i], NULL, entityID))
				return (UINT32_MAX);
	}

	if (!GEAddToSparseSet(ecs->entities, &flags, entityID))
		return (UINT32_MAX);
	return (entityID);
}

void	GEDeleteEntity(GEECS *ecs, u32 entityID)
{
	u32	*entityFlags;

	ASSERT(ecs,
		"Trying to delete entity from NULL ecs\n");

	entityFlags = GEGetFlags(ecs, entityID);
	ASSERT(entityFlags,
		"Trying to delete non existent entity\n");

	for (u32 i = 0; i < ecs->components->count; i++)
	{
		u32	compID = ecs->components->dense[i];

		if (*entityFlags & compID)
			GERemoveFromSparseSet(ecs->components->comp[i], entityID);
	}
}

u32	*GEGetFlags(GEECS *ecs, u32 entityID)
{
	ASSERT(ecs,
		"Failed to get flags, given ecs is NULL\n");

	return (ecs->entities->comp[ecs->entities->sparse[entityID]]);
}

void	*GEGetComponent(GEECS *ecs, u32 compID, u32 entityID)
{
	GESparseSet	*ss;

	ASSERT(ecs,
		"Trying to get a components from a NULL ecs\n");

	ss = ecs->components->comp[ecs->components->sparse[compID]];
	ASSERT(ss,
		"Trying to get a non existent comp from the ecs\n");

	return (ss->comp[ss->sparse[entityID]]);
}

void	GEDestroyECS(void *vecs)
{
	GEECS	*ecs;

	ecs = vecs;
	ASSERT(ecs,
		"Trying to destroy a NULL sparse set\n");

	_clearSS(ecs->systems);
	_clearSS(ecs->entities);
	_clearSS(ecs->components);
}

GEComponent	GECreateComponent(u32 compID, size_t compSize, void *(*defaultCreator)(void), void (*defaultRemover)(void *))
{
	return ((GEComponent){ compID, compSize, defaultCreator, defaultRemover });
}

GESystem	GECreateSystem(u32 requiredFlags, void (*action)(GEECS *ecs, u32 entityID))
{
	return ((GESystem){ requiredFlags, action });
}
