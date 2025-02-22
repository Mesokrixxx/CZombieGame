#include "GEPrivate.h"

f32	aimedFPS = 60;
f32	currentFPS = 0;

void	GECreateInstance(GEInstance *instance, char *title, iVec2 size, GEProjection projection)
{
	if (!instance)
		return ;

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

	instance->size = size;
	instance->bgColor = GE_COLOR_WHITE;
}

void	GELaunchInstance(GEInstance *instance)
{
	SDL_Event	ev;
	f32			deltaTime;	

	if (!instance)
		return ;

	instance->running = true;
	while (instance->running)
	{
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
				instance->running = false;
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
					system->action(instance->ecs, instance->ecs->entities->sparse[j]);
			}
		}

		SDL_GL_SwapWindow(instance->window);

		GEUpdateDTAndCapFPS(aimedFPS);
		deltaTime = GEGetDeltaTime();

		if (instance->debugMode)
			currentFPS = 1.0f / deltaTime;
	}
}

void	GEDestroyInstance(GEInstance *instance)
{
	if (!instance)
		return ;

	GEDestroyECS(instance->ecs);
	_free(instance->ecs);

	GEDestroyEventBus(instance->eventBus);
	_free(instance->eventBus);

	SDL_GL_DeleteContext(instance->glContext);
	SDL_DestroyWindow(instance->window);
	SDL_Quit();

	GEPAllocStats();
}
