#ifndef GAMEENGINE_EVENTS_H
# define GAMEENGINE_EVENTS_H

# include "vars.h"

# define EVENTTYPE_CHUNK_SIZE 24

typedef struct EventType {
	u32		typeID;
	char	*typeName;
	void	*(*defaultCreator)(void);
	void	(*defaultRemover)(void *);
}	EventType;

void		freeEventInRegistry(void *et);
EventType	*GetEventType(char *typeName, u32 *typeID);

typedef struct EventListener{
	u32						typeID;
	void					(*callback)(void *data);
	struct EventListener	*next;
}	EventListener;

void	DestroyEventBus(EventListener *eb);

typedef struct Event {
	u32		type;
	void	*data;
}	Event;

#endif