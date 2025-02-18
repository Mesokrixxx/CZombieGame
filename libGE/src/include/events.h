#ifndef GAMEENGINE_EVENTS_H
# define GAMEENGINE_EVENTS_H

# include "vars.h"

typedef enum {
	KEYDOWN_EVTP,
	KEYUP_EVTP,
	MOUSEDOWN_EVTP,
	MOUSEUP_EVTP,
	SCROLL_EVTP,
	QUIT_EVTP,
	DEFAULT_EVENT_TYPE_COUNT,
}	EventTypeID;

# define EVENTTYPE_CHUNK_SIZE 24

typedef struct EventType {
	u32		typeID;
	void	*(*defaultCreator)(void);
	void	(*defaultRemover)(void *);
}	EventType;

EventType	*GetEventType(u32 evtp);

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