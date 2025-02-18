#ifndef GAMEENGINE_ECS_H
# define GAMEENGINE_ECS_H

# include "vars.h"

# define ENTITY_CHUNK_SIZE		128
# define COMPONENT_CHUNK_SIZE	24
# define SYSTEM_CHUNK_SIZE		24

typedef enum {
	FLAGS_CMP,

	//...

	DEFAULT_COMPONENT_COUNT,
}	Component;

void	*GetComponent(u32 comp, u32 entityID);

typedef struct System {
	void	(*update)(u32, f32);
	u32		requiredFlags;
}	System;

typedef struct ECS {
	SparseSet	*comps;
	System		*systems;
	u32			systemsCount;
	u32			entityCount;
}	ECS;

ECS		*CreateECS();
void	DestroyECS(ECS *ecs);

Bool	InitDefaultComponents();

#endif