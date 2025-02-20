#include "GE.h"

extern Instance	*instance;

EventBus	*CreateEventBus()
{
	EventBus	*eb;

	eb = _malloc(sizeof(EventBus));
	if (!eb)
		return ((void)LOG("Failed to allocate for event bus\n"), NULL);
	
	eb->eventTypes = _malloc(sizeof(SparseSet));
	if (!eb->eventTypes)
		return ((void)LOG("Failed to allocate for event types in event bus\n"),
			_free(eb), NULL);
	if (!CreateSparseSet(eb->eventTypes, sizeof(EventType), EVENTTYPE_CHUNK_SIZE, NULL, NULL))
		return ((void)LOG("Failed to create sparseset for event types in event bus\n"),
			_free(eb->eventTypes), _free(eb), NULL);

	eb->listeners = _malloc(sizeof(SparseSet));
	if (!eb->listeners)
		return ((void)LOG("Failed to allocate for event listeners in event bus\n"),
			DestroySparseSet(eb->eventTypes), _free(eb), NULL);
	if (!CreateSparseSet(eb->listeners, sizeof(SparseSet), EVENTLISTENERS_CHUNK_SIZE, NULL, DestroySparseSet))
		return ((void)LOG("Failed to create sparseset for event listeners in event bus\n"),
			DestroySparseSet(eb->eventTypes), _free(eb->eventTypes), _free(eb), NULL);

	return (eb);
}

void	DestroyEventBus(EventBus *eb)
{
	DestroySparseSet(eb->listeners);
	_free(eb->listeners);
	DestroySparseSet(eb->eventTypes);
	_free(eb->eventTypes);
	_free(eb);
}

Bool	RegisterEventType(u32 evtp, void *(*defaultCreator)(void), void (*defaultRemover)(void *))
{
	EventType	et;

	if (!instance || !instance->eventBus || !instance->eventBus->eventTypes)
		return ((void)LOG("Can't create a new event type: instance not yet created\n"), false);

	et.defaultCreator = defaultCreator;
	et.defaultRemover = defaultRemover;

	et.typeID = evtp;
	if (!AddToSparseSet(instance->eventBus->eventTypes, &et, et.typeID))
		return ((void)LOG("Failed to register a new event type: failure when adding it to the registry\n"),
			false);
		
	SparseSet	ss;

	if (!CreateSparseSet(&ss, sizeof(EventListener), EVENTLISTENERS_CHUNK_SIZE, NULL, NULL))
		return ((void)LOG("Failed to create sparseset for event listeners\n"), false);
	if (!AddToSparseSet(instance->eventBus->listeners, &ss, et.typeID))
		return ((void)LOG("Failed to register a new event type: failure when adding it to the registry\n"),
			DestroySparseSet(&ss), false);

	return (true);
}

Event	*NewEvent(u32 evtp)
{
	Event		*e;
	EventType	*et;

	et = GetEventType(evtp);
	if (!et)
		return ((void)LOG("Failed to create a new event: didn't found event type\n"), NULL);

	e = _malloc(sizeof(Event));
	if (!e)
		return ((void)LOG("Failed to allocate for a new event\n"), NULL);

	e->type = et->typeID;
	e->data = et->defaultCreator ? et->defaultCreator() : NULL;
	return (e);
}

void	DestroyEvent(Event *e)
{
	EventType	*et;
	
	if (!e)
		return ((void)LOG("Faile to destroy NULL event\n"));
	
	et = GetEventType(e->type);
	if (!et)
		return ((void)LOG("Failed to destroy event: associated type not found\n"));

	if (et->defaultRemover)
		et->defaultRemover(e->data);
	_free(e);
}

Bool	NewEventListener(u32 evtp, void (*callback)(void *data))
{
	EventListener	el;
	EventType		*et;
	SparseSet		*ss;

	et = GetEventType(evtp);
	if (!et)
		return ((void)LOG("Failed to create event listnener, event type not found\n"), false);
	
	el.callback = callback;
	ss = instance->eventBus->listeners->comp[instance->eventBus->listeners->sparse[et->typeID]];
	if (!AddToSparseSet(ss, &el, ss->count))
		return ((void)LOG("Failed to create event listnener, failure when adding it to the registry\n"),
			false);
	
	return (true);
}

void	PublishEvent(Event *e)
{
	SparseSet	*ss;
	
	if (!e)
		return ((void)LOG("Failed to publish NULL event\n"));

	ss = instance->eventBus->listeners->comp[instance->eventBus->listeners->sparse[e->type]];
	for (u32 i = 0; i < ss->count; i++)
	{
		EventListener	*el = ss->comp[i];

		if (el->callback)
			el->callback(e->data);
	}		
}

EventType	*GetEventType(u32 evtp)
{	
	SparseSet	*ss;

	ss = instance->eventBus->eventTypes;
	return (ss->comp[ss->sparse[evtp]]);
}
