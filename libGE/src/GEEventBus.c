#include "GEPrivate.h"

static void	_clearSS(void *ss) { GEDestroySparseSet(ss); _free(ss); }
static void *_defaultMouseclickCreator();
static void *_defaultScancodeCreator();
static void *_defaultScrollCreator();

GEEventListener	GECreateEventListener(void (*callback)(void *instance, void *data, u32 entityID), u32 listenerID)
{
	return ((GEEventListener){ callback, listenerID });
}

GEEventType	GECreateEventType(u32 eventTypeID, void *(*defaultCreator)(void), void (*defaultRemover)(void *))
{
	return ((GEEventType){ eventTypeID, defaultCreator, defaultRemover });
}

void	GECreateEvent(GEEventBus *eventBus, GEEvent *event, u32 eventType)
{
	ASSERT(eventBus,
		"Trying to create an event but given eventbus is NULL\n");

	event->eventType = eventType;
	event->eventData = NULL;
	for (u32 i = 0; i < eventBus->eventTypeCount; i++)
	{
		if (eventBus->eventTypes[i].eventType == eventType)
		{
			if (eventBus->eventTypes[i].defaultCreator)
				event->eventData = eventBus->eventTypes[i].defaultCreator();
			break ;
		}
		ASSERT(i != eventBus->eventTypeCount - 1,
			"Trying to create an event with non existent event type\n");
	}
}

void	GEDestroyEvent(GEEventBus *eventBus, GEEvent *event)
{
	for (u32 i = 0; i < eventBus->eventTypeCount; i++)
	{
		if (eventBus->eventTypes[i].eventType == event->eventType)
		{
			if (eventBus->eventTypes[i].defaultRemover)
				eventBus->eventTypes[i].defaultRemover(event->eventData);
			break ;
		}
	}
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

	eventBus->eventTypes = _malloc(sizeof(GEEventType) * GE_EVENTTYPE_CHUNK_SIZE);
	if (!eventBus->eventTypes)
		return (_clearSS(eventBus->listeners), false);
	eventBus->eventTypeCount = 0;

	defaultContent =
		GERegisterEventType(eventBus, GECreateEventType(
			GE_EVENT_TYPE_MOUSEDOWN, _defaultMouseclickCreator, GEPFree))
		&& GERegisterEventType(eventBus, GECreateEventType(
			GE_EVENT_TYPE_MOUSEUP, _defaultMouseclickCreator, GEPFree))
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

	if (eventBus->eventTypeCount > 0 && eventBus->eventTypeCount % GE_EVENTTYPE_CHUNK_SIZE == 0)
	{
		GEEventType	*tempRealloc;

		tempRealloc = _realloc(eventBus->eventTypeCount * sizeof(GEEventType),
			(eventBus->eventTypeCount + GE_EVENTTYPE_CHUNK_SIZE) * sizeof(GEEventType),
			eventBus->eventTypes)
		if (!tempRealloc)
			return (false);

		eventBus->eventTypes = tempRealloc;
	}

	eventBus->eventTypes[eventBus->eventTypeCount] = eventType;
	eventBus->eventTypeCount++;

	if (!GECreateSparseSet(&ss, sizeof(GEEventListener), GE_EVENT_LISTENERS_CHUNK_SIZE, NULL, NULL))
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
	_free(eventBus->eventTypes);
}

void	GEPublishEvent(void *vinstance, GEEvent event)
{
	GESparseSet	*ss;
	GEInstance	*instance;

	ASSERT(instance,
		"Trying to publish an event but given instance is NULL\n");

	instance = vinstance;
	ss = GEGetFromSparseSet(instance->eventBus->listeners, event.eventType);
	ASSERT(ss,
		"Trying to publish an event to a non existent event type\n");

	for (u32 i = 0; i < ss->count; i++)
	{
		GEEventListener	*listener = ss->comp[i];
	
		listener->callback(vinstance, event.eventData, listener->linkedEntityID);
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
	SDL_Scancode	scancode;
	SDL_Scancode	*scancode_ptr;

	scancode = SDL_SCANCODE_0;
	scancode_ptr = &scancode;
	return (scancode_ptr);
}

static void *_defaultScrollCreator()
{
	iVec2	scroll;
	iVec2	*scroll_ptr;

	scroll = (iVec2){ 0 };
	scroll_ptr = &scroll;
	return (scroll_ptr);
}
