#ifndef GAME_ENGINE_ECS_H
# define GAME_ENGINE_ECS_H

# include "GEVars.h"
# include "GESparseSet.h"

typedef enum {
	GE_COMP_FLAG,
	GE_COMP_COUNT,
}	GEDefaultComponentTypes;

typedef struct {
	GESparseSet	*entities;
	GESparseSet	*components;
	GESparseSet	*systems;
}	GEECS;

typedef struct {
	u32		requiredFlags;
	void	(*action)(void *instance, u32 entityID, f32 dt);
}	GESystem;

GESystem	GECreateSystem(u32 requiredFlags, void (*action)(void *instance, u32 entityID, f32 dt));

typedef struct {
	u32		compID;
	size_t	compSize;
	void	*(*defaultCreator)(void);
	void	(*defaultRemover)(void *);
}	GEComponent;

GEComponent	GECreateComponent(u32 compID, size_t compSize, void *(*defaultCreator)(void), void (*defaultRemover)(void *));

bool	GECreateECS(GEECS *ecs);
bool	GERegisterComponent(GEECS *ecs, GEComponent comp);
bool	GERegisterSystem(GEECS *ecs, GESystem system);
u32		GECreateEntity(GEECS *ecs, u32 flags);
void	GEDeleteEntity(GEECS *ecs, u32 entityID);
u32		*GEGetFlags(GEECS *ecs, u32 entityID);
void	*GEGetComponent(GEECS *ecs, u32 compID, u32 entityID);
void	GEDestroyECS(void *ecs);

#endif