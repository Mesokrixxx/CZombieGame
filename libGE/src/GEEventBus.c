#include "GEPrivate.h"

static void	_clearSS(void *ss) { GEDestroySparseSet(ss); _free(ss); }
static void *_defaultMouseclickCreator();
static void *_defaultScancodeCreator();
static void *_defaultScrollCreator();

GEEventListener	GECreateEventListener(void (*callback)(void *data, u32 entityID), u32 listenerID)
{
	return ((GEEventListener){ callback, listenerID });
}

GEEventType	GECreateEventType(u32 eventTypeID, void *(*defaultCreator)(void), void (*defaultRemover)(void *))
{
	return ((GEEventType){ eventTypeID, defaultCreator, defaultRemover });
}

bool	GECreateEventBus(GEEventBus *eventBus)
{
	bool	defaultContent;

	ASSERT(eventBus,
		"Trying to create an event bus but given eventbus is NULL\n");

	eventBus->listeners = _malloc(sizeof(GESparseSet));
	if (!eventBus->listeners)
		return (false);
	if (!GECreateSparseSet(eventBus->listeners, sizeof(GESparseSet), GE_EVENTTYPE_CHUNK_SIZE, NULL, _clearSS))
		return (_free(eventBus->listeners), false);

	defaultContent =
		GERegisterEventType(eventBus, GECreateEventType(
			GE_EVENT_TYPE_MOUSEDOWN, _defaultMouseclickCreator, NULL))
		&& GERegisterEventType(eventBus, GECreateEventType(
			GE_EVENT_TYPE_MOUSEUP, _defaultMouseclickCreator, NULL))
		&& GERegisterEventType(eventBus, GECreateEventType(
			GE_EVENT_TYPE_KEYDOWN, _defaultScancodeCreator, NULL))
		&& GERegisterEventType(eventBus, GECreateEventType(
			GE_EVENT_TYPE_KEYUP, _defaultScancodeCreator, NULL))
		&& GERegisterEventType(eventBus, GECreateEventType(
			GE_EVENT_TYPE_SCROLL, _defaultScrollCreator, NULL))
		&& GERegisterEventType(eventBus, GECreateEventType(
			GE_EVENT_TYPE_QUIT, NULL, NULL));

	return (defaultContent);
}

bool	GEAddEventListener(GEEventBus *eventBus, u32 eventTypeID, GEEventListener listener)
{
	GESparseSet	*ss;

	ASSERT(eventBus,
		"Trying to add an event listener but given eventbus is NULL\n");

	ASSERT(listener.callback,
		"Trying to add an event listener with a NULL callback\n");

	ss = GEGetFromSparseSet(eventBus->listeners, eventTypeID);
	ASSERT(ss,
		"Trying to add an event listener listening to a non existent event type\n");

	return (GEAddToSparseSet(ss, &listener, ss->count));
}

bool	GERegisterEventType(GEEventBus *eventBus, GEEventType eventType)
{
	GESparseSet	ss;

	ASSERT(eventBus,
		"Trying to add an event listener but given eventbus is NULL\n");

	if (!GECreateSparseSet(&ss, sizeof(GEEventListener), GE_EVENT_LISTENERS_CHUNK_SIZE, eventType.defaultCreator, eventType.defaultRemover))
		return (false);

	if (!GEAddToSparseSet(eventBus->listeners, &ss, eventType.eventType))
		return (false);

	return (true);
}

void	GEDestroyEventBus(GEEventBus *eventBus)
{
	ASSERT(eventBus,
		"Trying to destroy an event bus but given eventbus is NULL\n");

	GEDestroySparseSet(eventBus->listeners);
	_free(eventBus->listeners);
}

void	GEPublishEvent(GEEventBus *eventBus, GEEvent event)
{
	GESparseSet	*ss;

	ASSERT(eventBus,
		"Trying to publish an event but given eventbus is NULL\n");

	ss = GEGetFromSparseSet(eventBus->listeners, event.eventType);
	ASSERT(ss,
		"Trying to publish an event to a non existent event type\n");

	for (u32 i = 0; i < ss->count; i++)
	{
		GEEventListener	*listener = ss->comp[i];
	
		listener->callback(event.eventData, listener->linkedEntityID);
	}
}

static void *_defaultMouseclickCreator()
{
	MouseEvent	*mouse;

	mouse = _malloc(sizeof(MouseEvent));
	if (!mouse) return (NULL);

	mouse->pos = (iVec2){ 0 };
	mouse->button = SDL_BUTTON_LEFT;
	return (mouse);
}

static void *_defaultScancodeCreator()
{
	SDL_Scancode	*scancode;

	scancode = _malloc(sizeof(SDL_Scancode));
	if (!scancode)
		return (NULL);

	*scancode = SDL_SCANCODE_0;
	return (scancode);
}

static void *_defaultScrollCreator()
{
	iVec2	*scroll;

	scroll = _malloc(sizeof(iVec2));
	if (!scroll)
		return (NULL);

	scroll->x = 0;
	scroll->y = 0;
	return (scroll);
}
