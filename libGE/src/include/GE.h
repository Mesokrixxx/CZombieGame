#ifndef GAMEENGINE_H
# define GAMEENGINE_H

# include "vars.h"
# include "sparseset.h"
# include "events.h"

typedef struct Instance {
	SDL_Window		*window;
	SDL_GLContext	glContext;
	SparseSet		*eventTypeRegistry;
	EventListener	*eventBus;
	Bool			running;
}	Instance;

// Instance
Instance	*CreateInstance(const char *title, u32 width, u32 height);
void		LaunchInstance();
void		DestroyInstance();

// Events
Bool		RegisterEventType(char *typeName, void *(*defaultCreator)(void), void (*defaultRemover)(void *));
Event		*CreateEvent(char *typeName);
void		DestroyEvent(Event *e);
Bool		NewEventListener(char *typeName, void (*callback)(void *data));
void		PublishEvent(Event *e);

// Default
Bool		InitDefaultContent();

#endif