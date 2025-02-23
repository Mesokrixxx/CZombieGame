#include "GEPrivate.h"

GEInstance	*activeInstance;

f32		aimedFPS = 60;
f32		currentFPS = 0;

iVec2	mousePos;

static void _defaultQuitMethod(GEInstance *instance);
static void	_quitListener(void *, void *, u32);
static void	_callQuitFuncOfInstance(void *data, u32 entityID);

void	GECreateInstance(GEInstance *instance, char *title, iVec2 size, GEProjection projection)
{
	if (!instance)
		return ;

	instance->size = size;
	instance->bgColor = GE_COLOR_WHITE;
	instance->quitMethod = _defaultQuitMethod;

	ASSERT(!SDL_Init(SDL_INIT_VIDEO),
		"Failed to initialize SDL: %s\n", SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	instance->window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1),
		size.x, size.y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	ASSERT(instance->window,
		"Failed to create SDL Window: %s\n", SDL_GetError());

	instance->glContext = SDL_GL_CreateContext(instance->window);
	ASSERT(instance->glContext,
		"Failed to create OpenGL context: %s\n", SDL_GetError());

	// Deactivate VSYNC
	SDL_GL_SetSwapInterval(0);

	// Activate transparency with alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ASSERT(glewInit() == GLEW_OK,
		"Failed to initialize GLEW\n");

	instance->ecs = _malloc(sizeof(GEECS));
	ASSERT(instance->ecs && GECreateECS(instance->ecs),
		"Failed to create ECS\n");

	instance->eventBus = _malloc(sizeof(GEEventBus));
	ASSERT(instance->eventBus && GECreateEventBus(instance->eventBus),
		"Failed to create event bus\n");

	instance->graphics = _malloc(sizeof(GEGraphics));
	ASSERT(instance->graphics && GEInitGraphics(instance->graphics, instance->size, projection),
		"Failed to create graphics for instance\n");

	bool defaultContent = 
		GEAddEventListener(instance->eventBus, GE_EVENT_TYPE_QUIT,
			GECreateEventListener(_quitListener, 0));

	ASSERT(defaultContent,
		"Failed to initialize default content of instance\n");

	activeInstance = instance;
}

void	GELaunchInstance(GEInstance *instance)
{
	SDL_Event	ev;
	f32			deltaTime = 0;	
	
	GEEvent		keydownEvent;
	GEEvent		keyupEvent;
	GEEvent		mouseDownEvent;
	GEEvent		mouseUpEvent;
	GEEvent		scrollEvent;
	GEEvent		quitEvent;

	if (!instance)
		return ;

	GECreateEvent(instance->eventBus, &keydownEvent, GE_EVENT_TYPE_KEYDOWN);
	GECreateEvent(instance->eventBus, &keyupEvent, GE_EVENT_TYPE_KEYUP);
	GECreateEvent(instance->eventBus, &mouseDownEvent, GE_EVENT_TYPE_MOUSEDOWN);
	GECreateEvent(instance->eventBus, &mouseUpEvent, GE_EVENT_TYPE_MOUSEUP);
	GECreateEvent(instance->eventBus, &scrollEvent, GE_EVENT_TYPE_SCROLL);
	GECreateEvent(instance->eventBus, &quitEvent, GE_EVENT_TYPE_QUIT);

	instance->running = true;
	while (instance->running)
	{
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
				GEPublishEvent(instance, quitEvent);
			else if (ev.type == SDL_MOUSEBUTTONUP)
			{
				((MouseEvent *)mouseUpEvent.eventData)->button = ev.button.button;
				((MouseEvent *)mouseUpEvent.eventData)->pos = newIVec2(ev.button.x, ev.button.y);
				GEPublishEvent(instance, mouseUpEvent);
			}
			else if (ev.type == SDL_KEYUP)
			{
				keyupEvent.eventData = &ev.key.keysym.scancode;
				GEPublishEvent(instance, keyupEvent);
			}
			else if (ev.type == SDL_MOUSEWHEEL)
			{
				*(iVec2 *)scrollEvent.eventData = newIVec2(ev.wheel.x, ev.wheel.y);
				GEPublishEvent(instance, scrollEvent);
			}
		}

		const u8	*keys = SDL_GetKeyboardState(NULL);
		u32			mouse = SDL_GetMouseState(&mousePos.x, &mousePos.y);
		
		for (SDL_Scancode i = 0; i < SDL_NUM_SCANCODES; i++)
		{
			if (keys[i])
			{
				keydownEvent.eventData = &i;
				GEPublishEvent(instance, keydownEvent);
			}
		}

		if (mouse & SDL_BUTTON_LMASK)
		{
			((MouseEvent *)mouseDownEvent.eventData)->button = SDL_BUTTON_LEFT;
			((MouseEvent *)mouseDownEvent.eventData)->pos = mousePos;
			GEPublishEvent(instance, mouseDownEvent);
		}
		
		if (mouse & SDL_BUTTON_RMASK)
		{
			((MouseEvent *)mouseDownEvent.eventData)->button = SDL_BUTTON_RIGHT;
			((MouseEvent *)mouseDownEvent.eventData)->pos = mousePos;
			GEPublishEvent(instance, mouseDownEvent);
		}

		if (mouse & SDL_BUTTON_MMASK)
		{
			((MouseEvent *)mouseDownEvent.eventData)->button = SDL_BUTTON_MIDDLE;
			((MouseEvent *)mouseDownEvent.eventData)->pos = mousePos;
			GEPublishEvent(instance, mouseDownEvent);
		}

		glClearColor(instance->bgColor.r, instance->bgColor.g,
			instance->bgColor.b, instance->bgColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (u32 i = 0; i < instance->ecs->systems->count; i++)
		{
			GESystem	*system = instance->ecs->systems->comp[i];
		
			for (u32 j = 0; j < instance->ecs->entities->count; j++)
			{
				u32	*flags = instance->ecs->entities->comp[j];

				if ((*flags & system->requiredFlags) == system->requiredFlags)
					system->action(instance, instance->ecs->entities->sparse[j], deltaTime);
			}
		}

		SDL_GL_SwapWindow(instance->window);

		GEUpdateDTAndCapFPS(aimedFPS);
		deltaTime = GEGetDeltaTime();

		if (instance->debugMode)
			currentFPS = 1.0f / deltaTime;
	}

	GEDestroyEvent(instance->eventBus, &keydownEvent);
	GEDestroyEvent(instance->eventBus, &keyupEvent);
	GEDestroyEvent(instance->eventBus, &mouseDownEvent);
	GEDestroyEvent(instance->eventBus, &mouseUpEvent);
	GEDestroyEvent(instance->eventBus, &scrollEvent);
	GEDestroyEvent(instance->eventBus, &quitEvent);
}

void	GEDestroyInstance(GEInstance *instance)
{
	if (!instance)
		return ;

	GEDestroyECS(instance->ecs);
	_free(instance->ecs);

	GEDestroyEventBus(instance->eventBus);
	_free(instance->eventBus);

	GEDestroyGraphics(instance->graphics);
	_free(instance->graphics);

	SDL_GL_DeleteContext(instance->glContext);
	SDL_DestroyWindow(instance->window);
	SDL_Quit();

	GEPAllocStats();
}

void	GESetBackgroundColor(GEInstance *instance, Color bgCol)
{
	ASSERT(instance,
		"Trying to change background color of a NULL instance\n");

	instance->bgColor = bgCol;
}

void	GESetQuitMethod(GEInstance *instance, void (*newQuitMethod)(GEInstance *instance))
{
	ASSERT(instance,
		"Trying to change quit method of a NULL instance\n");

	instance->quitMethod = newQuitMethod;
}

GEInstance	*GEPGetActiveInstance()
{
	return (activeInstance);
}

static void	_defaultQuitMethod(GEInstance *instance)
{
	instance->running = false;
}

static void _quitListener(void *vinstance, void *nuldata, u32 nulID)
{
	GEInstance	*instance = vinstance;

	instance->quitMethod(instance);
	(void)nulID;
	(void)nuldata;
}

static void	_callQuitFuncOfInstance(void *data, u32 entityID)
{
	GEInstance	*instance = data;

	instance->quitMethod(instance);
	entityID = 0;
}
