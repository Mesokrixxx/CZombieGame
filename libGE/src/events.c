#include "GE.h"

extern Instance	*instance;

void	freeEventInRegistry(void *et)
{
	_free(((EventType *)et)->typeName);
}

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

Bool	RegisterEventType(char *typeName, void *(*defaultCreator)(void), void (*defaultRemover)(void *))
{
	EventType	et;

	if (!instance || !instance->eventTypeRegistry)
		return ((void)LOG("Can't create a new event type: instance not yet created (%s)\n", typeName), false);
	
	if (!typeName)
		return ((void)LOG("Can't create a new event type: typeName can't be NULL\n"), false);	
	et.typeName = _malloc((strlen(typeName) + 1) * sizeof(char));
	if (!et.typeName)
		return ((void)LOG("Failed to register a new event type: allocation for name failed (%s)\n", typeName), false);
	strcpy(et.typeName, typeName);

	et.defaultCreator = defaultCreator;
	et.defaultRemover = defaultRemover;

	et.typeID = instance->eventTypeRegistry->count;
	if (!AddToSparseSet(instance->eventTypeRegistry, &et, et.typeID))
		return ((void)LOG("Failed to register a new event type: failure when adding it to the registry (%s)\n", typeName),
			_free(et.typeName), false);

	return (true);
}

Event	*CreateEvent(char *typeName)
{
	Event		*e;
	EventType	*et;

	et = GetEventType(typeName, NULL);
	if (!et)
		return ((void)LOG("Failed to create a new event: didn't found event type '%s'\n", typeName), NULL);

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
	
	et = GetEventType(NULL, &e->type);
	if (!et)
		return ((void)LOG("Failed to destroy event: associated type not found\n"));

	if (et->defaultRemover)
		et->defaultRemover(e->data);
	_free(e);
}

Bool	NewEventListener(char *typeName, void (*callback)(void *data))
{
	EventListener	*el;
	EventType		*et;

	et = GetEventType(typeName, NULL);
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
	LOG("No listeners found for published event\n");
}

EventType	*GetEventType(char *typeName, u32 *typeID)
{
	if (!typeName && !typeID)
		return((void)LOG("Trying to find event type with both args set to NULL\n"), NULL);
	
	for (u32 i = 0; i < instance->eventTypeRegistry->count; i++)
	{
		EventType	*et = instance->eventTypeRegistry->comp[i];

		if (typeID)
		{
			if (*typeID == et->typeID)
				return (et);
		}
		else if (typeName)
		{
			if (strcmp(typeName, et->typeName) == 0)
				return (et);
		}
	}
	return((void)LOG("No event type found with set args: %s, %d\n", typeName, *typeID), NULL);
}
