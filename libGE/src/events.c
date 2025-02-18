#include "GE.h"

extern Instance	*instance;

void	DestroyEventBus(EventListener *eb)
{
	EventListener	*temp;

	if (!eb)
		return ((void)LOG("Tried to destroy NULL event bus\n"));

	temp = eb->next;
	while (temp)
	{
		_free(eb);
		eb = temp;
		temp = eb->next;
	}
	_free(eb);
}

Bool	RegisterEventType(u32 evtp, void *(*defaultCreator)(void), void (*defaultRemover)(void *))
{
	EventType	et;

	if (!instance || !instance->eventTypeRegistry)
		return ((void)LOG("Can't create a new event type: instance not yet created\n"), false);

	et.defaultCreator = defaultCreator;
	et.defaultRemover = defaultRemover;

	et.typeID = evtp;
	if (!AddToSparseSet(instance->eventTypeRegistry, &et, et.typeID))
		return ((void)LOG("Failed to register a new event type: failure when adding it to the registry\n"),
			false);

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
	EventListener	*el;
	EventType		*et;

	et = GetEventType(evtp);
	if (!et)
		return ((void)LOG("Failed to create event listnener, event type not found\n"), false);
	
	el = _malloc(sizeof(EventListener));
	if (!el)
		return ((void)LOG("Failed to allocate for new event listener\n"), false);
	el->typeID = et->typeID;
	el->callback = callback;	
	el->next = NULL;

	if (!instance->eventBus)
		instance->eventBus = el;
	else
	{
		EventListener	*temp;

		temp = instance->eventBus->next;
		instance->eventBus->next = el;
		el->next = temp;
	}
	return (true);
}

void	PublishEvent(Event *e)
{
	EventListener	*eb;
	
	if (!e)
		return ((void)LOG("Failed to publish NULL event\n"));

	eb = instance->eventBus;
	while (eb)
	{
		if (eb->typeID == e->type)
		{
			if (eb->callback)
				eb->callback(e->data);
			return ;
		}
		eb = eb->next;
	}
}

EventType	*GetEventType(u32 evtp)
{	
	SparseSet	*ss;

	ss = instance->eventTypeRegistry;
	return (ss->comp[ss->sparse[evtp]]);
}
