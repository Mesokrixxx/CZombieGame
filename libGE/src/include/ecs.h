#ifndef GAMEENGINE_ECS_H
# define GAMEENGINE_ECS_H

# include "vars.h"

# define ENTITY_CHUNK_SIZE		128
# define COMPONENT_CHUNK_SIZE	24

typedef enum {
	FLAGS_CMP,

	//...

	DEFAULT_COMPONENT_COUNT,
}	Component;

u32		GetFlags(u32 entityID);

typedef struct ECS {
	SparseSet	*comps;
	u32			entityCount;
}	ECS;

ECS		*CreateECS();
void	DestroyECS(ECS *ecs);

Bool	InitDefaultComponents();

#endif