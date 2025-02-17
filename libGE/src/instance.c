#include "GE.h"

f32			AimedFPS = 60;
f32			CurrentFPS;
Instance	*instance;

Instance	*CreateInstance(const char *title, u32 width, u32 height)
{	
	instance = _malloc(sizeof(Instance));
	ASSERT(instance,
		"Failed to allocate for new instance\n");
	memset(instance, 0, sizeof(Instance));

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

	instance->eventTypeRegistry = _malloc(sizeof(SparseSet));
	ASSERT(instance->eventTypeRegistry,
		"Failed to create event type registry for new instance\n");

	ASSERT(CreateSparseSet(instance->eventTypeRegistry, sizeof(EventType), EVENTTYPE_CHUNK_SIZE, NULL, freeEventInRegistry),
		"Failed to create sparse set of eventTypeRegistry\n");
			
	instance->eventBus = NULL;

	instance->entities = CreateECS();
	ASSERT(instance->entities,
		"Failed to create ECS\n");

	ASSERT(InitDefaultContent(),
		"Failed to init all the default content\n");

	instance->running = true;
	return (instance);
}

void	LaunchInstance()
{
	SDL_Event	ev;

	Event	*keydownEvent = NewEvent("keydown_event");
	Event	*keyupEvent = NewEvent("keyup_event");
	Event	*mouseButtonDownEvent = NewEvent("mousedown_event");
	Event	*mouseButtonUpEvent = NewEvent("mouseup_event");
	Event	*scrollEvent = NewEvent("scroll_event");
	Event	*quitEvent = NewEvent("quit_event");

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
			else if (ev.type == SDL_MOUSEBUTTONUP)
			{
				mouseButtonUpEvent->data = &ev.button.button;
				PublishEvent(mouseButtonUpEvent);
			}
			else if (ev.type == SDL_MOUSEWHEEL)
			{
				scrollEvent->data = &((iVec2){ ev.wheel.x, ev.wheel.y });
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

		iVec2		mousePos;
		u32			mouseState = SDL_GetMouseState(&mousePos.x, &mousePos.y);
		static i8	leftButton = SDL_BUTTON_LEFT;
		static i8	rightButton = SDL_BUTTON_RIGHT;
		
		if (mouseState & SDL_BUTTON_LMASK)
		{
			mouseButtonDownEvent->data = &leftButton;
			PublishEvent(mouseButtonDownEvent);
		}
		if (mouseState & SDL_BUTTON_RMASK)
		{
			mouseButtonDownEvent->data = &rightButton;
			PublishEvent(mouseButtonDownEvent);
		}
		
		// update

		// Set bg color

		// render

		UpdateDeltaTime();
		f32 frameTime = GetDeltaTime();
		f32 targetFrameTime = 1.0f / AimedFPS;
		
		if (frameTime < targetFrameTime)
		{
			u32 delayTime = (u32)((targetFrameTime - frameTime) * 1000.0f);
			if (delayTime > 0)
				SDL_Delay(delayTime);
		}
		UpdateDeltaTime();
		CurrentFPS = 1.0f / GetDeltaTime();
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
	_free(instance->eventTypeRegistry);
	DestroyEventBus(instance->eventBus);
	DestroyECS(instance->entities);

	SDL_GL_DeleteContext(instance->glContext);
	SDL_DestroyWindow(instance->window);
	SDL_Quit();
	_free(instance);
}
