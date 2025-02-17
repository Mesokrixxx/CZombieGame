#ifndef GAMEENGINE_H
# define GAMEENGINE_H

# include "vars.h"
# include "sparseset.h"
# include "events.h"
# include "ecs.h"
# include "default.h"
# include "dtime.h"

typedef struct Instance {
	SDL_Window		*window;
	SDL_GLContext	glContext;
	SparseSet		*eventTypeRegistry;
	EventListener	*eventBus;
	ECS				*entities;
	Bool			running;
}	Instance;

// Instance
Instance	*CreateInstance(const char *title, u32 width, u32 height);
void		LaunchInstance();
void		DestroyInstance();

// Events
Bool		RegisterEventType(char *typeName, void *(*defaultCreator)(void), void (*defaultRemover)(void *));
Event		*NewEvent(char *typeName);
void		DestroyEvent(Event *e);
Bool		NewEventListener(char *typeName, void (*callback)(void *data));
void		PublishEvent(Event *e);

// Time
f32			GetDeltaTime();

// Entities
Bool		RegisterComponent(u32 compID, size_t compSize, void *(*defaultCreator)(void), void (*defaultRemover)(void *));
u32			CreateEntity(u32 flags);
void		RemoveEntity(u32 entityID);

#endif