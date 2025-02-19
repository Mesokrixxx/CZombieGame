#include "GE.h"

f32			AimedFPS = 60;
f32			CurrentFPS;
Instance	*instance;

Instance	*CreateInstance(const char *title, u32 width, u32 height, ProjType projType)
{	
	instance = _malloc(sizeof(Instance));
	ASSERT(instance,
		"Failed to allocate for new instance\n");
	memset(instance, 0, sizeof(Instance));

	ASSERT(!SDL_Init(SDL_INIT_VIDEO),
		"Failed to initialise SDL\n");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
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

	SDL_GL_SetSwapInterval(0);

	ASSERT(glewInit() == GLEW_OK,
		"Failed to init GLEW\n");

	instance->windowParam.h = height;
	instance->windowParam.w = width;

	instance->shaderPrograms = _malloc(sizeof(SparseSet));
	ASSERT(instance->shaderPrograms,
		"Failed to allocate for shaderProgram\n");
	ASSERT(CreateSparseSet(instance->shaderPrograms, sizeof(GLuint), SHADERPROGRAM_CHUNK_SIZE, NULL, DestroyShaderProgram),
		"Failed to create shader program sparse set\n");

	instance->projectionMatrice = _malloc(sizeof(f32) * 16);
	ASSERT(instance->projectionMatrice,
		"Failed to allocate for projection matrice \n");
	Mat4x4ToFloat(CreateProjectionMatrice(projType), instance->projectionMatrice);

	instance->eventTypeRegistry = _malloc(sizeof(SparseSet));
	ASSERT(instance->eventTypeRegistry,
		"Failed to create event type registry for new instance\n");

	ASSERT(CreateSparseSet(instance->eventTypeRegistry, sizeof(EventType), EVENTTYPE_CHUNK_SIZE, NULL, NULL),
		"Failed to create sparse set of eventTypeRegistry\n");
			
	instance->eventBus = NULL;

	instance->entities = CreateECS();
	ASSERT(instance->entities,
		"Failed to create ECS\n");

	instance->bgCol = BLACK;

	instance->VOs = _malloc(sizeof(SparseSet));
	ASSERT(instance->VOs,
		"Failed to create Vertex Object sparse set\n");
	
	ASSERT(CreateSparseSet(instance->VOs, sizeof(VertexObject), VERTEXOBJECT_CHUNK_SIZE, NULL, DestroyVertexObject),
		"Failed to create sparse set of eventTypeRegistry\n");

	instance->debugMode = false;
	instance->debugUI = CreateUIDebugger();
	ASSERT(instance->debugUI,
		"Failed to create UI debugger\n");

	ASSERT(InitDefaultContent(),
		"Failed to init all the default content\n");

	instance->running = true;
	return (instance);
}

void	LaunchInstance()
{
	SDL_Event	ev;
	SparseSet	*flagsSS;

	Event		*keydownEvent = NewEvent(KEYDOWN_EVTP);
	Event		*keyupEvent = NewEvent(KEYUP_EVTP);
	Event		*mouseButtonDownEvent = NewEvent(MOUSEDOWN_EVTP);
	Event		*mouseButtonUpEvent = NewEvent(MOUSEUP_EVTP);
	Event		*scrollEvent = NewEvent(SCROLL_EVTP);
	Event		*quitEvent = NewEvent(QUIT_EVTP);

	ASSERT(keydownEvent || keyupEvent || mouseButtonDownEvent || mouseButtonUpEvent || scrollEvent || quitEvent,
		"Failed to create base events\n");

	flagsSS = instance->entities->comps->comp[instance->entities->comps->sparse[FLAGS_CMP]];
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
		for (SDL_Scancode i = 0; i < SDL_NUM_SCANCODES; i++)
		{
			if (keyboardState[i])
			{
				SDL_Scancode *scancode_ptr = &i;
				keydownEvent->data = scancode_ptr;
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

		glClearColor(instance->bgCol.r, instance->bgCol.g,
			instance->bgCol.b, instance->bgCol.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float	dt = GetDeltaTime();
		for (u32 i = 0; i < instance->entities->systemsCount; i++)
		{
			System s = instance->entities->systems[i];

			for (u32 j = 0; j < flagsSS->count; j++)
			{
				u32 entityID = flagsSS->dense[j];
				u32 entityFlags = *(u32 *)GetComponent(FLAGS_CMP, entityID);

				
				if ((entityFlags & s.requiredFlags) == s.requiredFlags)
					s.update(entityID, dt);
			}
		}

		if (instance->debugMode)
			RenderDebugUI(instance->debugUI);

		SDL_GL_SwapWindow(instance->window);

		UpdateDeltaTimeNCapFPS();

		CurrentFPS = 1.0f / GetDeltaTime();
	}

	DestroyEvent(keydownEvent);
	DestroyEvent(keyupEvent);
	DestroyEvent(mouseButtonDownEvent);
	DestroyEvent(mouseButtonUpEvent);
	DestroyEvent(scrollEvent);
	DestroyEvent(quitEvent);
}

void	SetInstanceBGCol(Color c)
{
	instance->bgCol = c;
}

void	DestroyInstance()
{
	DestroySparseSet(instance->eventTypeRegistry);
	_free(instance->eventTypeRegistry);
	DestroyEventBus(instance->eventBus);
	DestroyECS(instance->entities);
	_free(instance->projectionMatrice);
	DestroySparseSet(instance->VOs);
	_free(instance->VOs);
	DestroySparseSet(instance->shaderPrograms);
	_free(instance->shaderPrograms);
	DestroyUIDebugger(instance->debugUI);
	SDL_GL_DeleteContext(instance->glContext);
	SDL_DestroyWindow(instance->window);
	SDL_Quit();
	_free(instance);
}
