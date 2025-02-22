#ifndef GAME_ENGINE_EVENT_BUS_H
# define GAME_ENGINE_EVENT_BUS_H

typedef enum {
	GE_EVENT_TYPE_MOUSEDOWN,
	GE_EVENT_TYPE_MOUSEUP,
	GE_EVENT_TYPE_KEYDOWN,
	GE_EVENT_TYPE_KEYUP,
	GE_EVENT_TYPE_SCROLL,
	GE_EVENT_TYPE_QUIT,
	GE_EVENT_TYPE_COUNT,
}	GEDefaultEventType;

# include "GEVars.h"
# include "GESparseSet.h"

typedef struct {
	void	*eventData;
	u32		eventType;
}	GEEvent;

typedef struct {
	u32		eventType;
	void	*(*defaultCreator)(void);
	void	(*defaultRemover)(void *);
}	GEEventType;

typedef struct {
	void	(*callback)(void *, u32);
	u32		linkedEntityID;
}	GEEventListener;

typedef struct {
	GESparseSet	*listeners;
	GEEventType	*eventTypes;
	u32			eventTypeCount;
}	GEEventBus;

GEEventListener	GECreateEventListener(void (*callback)(void *data, u32 entityID), u32 linkedEntityID);
GEEventType		GECreateEventType(u32 eventTypeID, void *(*defaultCreator)(void), void (*defaultRemover)(void *));

void			GECreateEvent(GEEventBus *eventBus, GEEvent *event, u32 eventType);
void			GEDestroyEvent(GEEventBus *eventBus, GEEvent *event);
bool			GECreateEventBus(GEEventBus *eventBus);
bool			GEAddEventListener(GEEventBus *eventBus,  u32 eventTypeID, GEEventListener listener);
bool			GERegisterEventType(GEEventBus *eventBus, GEEventType eventType);
void			GEDestroyEventBus(GEEventBus *eventBus);

void			GEPublishEvent(GEEventBus *eventBus, GEEvent event);

#endif
