#include "GE.h"

Instance	*instance;

Instance	*CreateInstance(const char *title, u32 width, u32 height)
{	
	instance = _malloc(sizeof(Instance));
	ASSERT(instance,
		"Failed to allocate for new instance\n");
	bzero(instance, sizeof(Instance));

	ASSERT(!SDL_Init(SDL_INIT_VIDEO),
		"Failed to initialise SDL\n");

	instance->window =
		SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1),
			width, height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	ASSERT(instance->window,
		"Failed to create a window for new instance\n");

	instance->glContext = SDL_GL_CreateContext(instance->window);
	ASSERT(instance->glContext,
		"Failed to create a glContext for new instance\n");

	instance->eventTypeRegistry = CreateSparseSet(sizeof(EventType), EVENTTYPE_CHUNK_SIZE, freeEventInRegistry);
	ASSERT(instance->eventTypeRegistry,
		"Failed to create event type registry for new instance\n");

	instance->eventBus = NULL;

	ASSERT(InitDefaultContent(),
		"Failed to init all the default content\n");

	instance->running = true;
	return (instance);
}

void	LaunchInstance()
{
	SDL_Event	ev;

	Event	*keydownEvent = CreateEvent("keydown_event");
	Event	*keyupEvent = CreateEvent("keyup_event");
	Event	*mouseButtonDownEvent = CreateEvent("mousedown_event");
	Event	*mouseButtonUpEvent = CreateEvent("mouseup_event");
	Event	*scrollEvent = CreateEvent("scroll_event");
	Event	*quitEvent = CreateEvent("quit_event");

	ASSERT(keydownEvent || keyupEvent || mouseButtonDownEvent || mouseButtonUpEvent || scrollEvent || quitEvent,
		"Failed to create base events\n");

	while (instance->running)
	{
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
				PublishEvent(quitEvent);
			else if (ev.type == SDL_KEYUP)
			{
				keyupEvent->data = &ev.key.keysym.scancode;
				PublishEvent(keyupEvent);
			}
			else if (ev.type == SDL_MOUSEBUTTONDOWN)
			{
				mouseButtonDownEvent->data = &ev.button.button;
				PublishEvent(mouseButtonDownEvent);
			}
			else if (ev.type == SDL_MOUSEBUTTONUP)
			{
				mouseButtonUpEvent->data = &ev.button.button;
				PublishEvent(mouseButtonUpEvent);
			}
			else if (ev.type == SDL_MOUSEWHEEL)
			{
				scrollEvent->data = &((Vec2){ ev.wheel.x, ev.wheel.y });
				PublishEvent(scrollEvent);
			}
		}

		const u8	*keyboardState = SDL_GetKeyboardState(NULL);

		for (u16 i = 0; i < SDL_NUM_SCANCODES; i++)
		{
			if (keyboardState[i])
			{
				keydownEvent->data = &i;
				PublishEvent(keydownEvent);
			}	
		}
		
		// update
		
		// Set bg color

		// render

		// update delta time
	}

	DestroyEvent(keydownEvent);
	DestroyEvent(keyupEvent);
	DestroyEvent(mouseButtonDownEvent);
	DestroyEvent(mouseButtonUpEvent);
	DestroyEvent(scrollEvent);
	DestroyEvent(quitEvent);
}

void	DestroyInstance()
{
	DestroySparseSet(instance->eventTypeRegistry);
	DestroyEventBus(instance->eventBus);

	SDL_GL_DeleteContext(instance->glContext);
	SDL_DestroyWindow(instance->window);
	SDL_Quit();
	_free(instance);
}
