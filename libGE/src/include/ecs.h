#ifndef GAMEENGINE_ECS_H
# define GAMEENGINE_ECS_H

# include "vars.h"

# define ENTITY_CHUNK_SIZE		128
# define COMPONENT_CHUNK_SIZE	24

typedef struct ComponentType {
	char	*typeName;
	u32		typeID;
	void	*(*defaultCreator)(void);
	void	(*defaultRemover)(void *);
}	ComponentType;

void			freeComponentInECS(void *data);

typedef struct ECS {
	SparseSet	*comps;
	u32			entityCount;
}	ECS;

ECS		*CreateECS();
void	DestroyECS(ECS *ecs);

Bool	InitDefaultComponents();

#endif