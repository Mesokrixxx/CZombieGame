#include "main.h"

ZombieGame	game;

int main(void)
{
	ASSERT(!SDL_Init(SDL_INIT_VIDEO),
		"Failed to init SDL: %s\n", SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	game.window.window =
		SDL_CreateWindow(
			"ZombieGame",
			SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1),
			DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	ASSERT(game.window.window,
		"Failed to create SDL window: %s\n", SDL_GetError());
	
	game.window.context =
		SDL_GL_CreateContext(
			game.window.window);
	ASSERT(game.window.context,
		"Failed to create default gl context: %s\n", SDL_GetError());

	game.window.width = DEFAULT_WINDOW_WIDTH;
	game.window.height = DEFAULT_WINDOW_HEIGHT;
	game.window.color = WHITE;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ASSERT(glewInit() == GLEW_OK,
		"Failed to initialize GLEW\n");
	
	game.running = true;
	while (game.running)
	{
		SDL_Event	ev;

		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
				game.running = false;
			else if (ev.type == SDL_WINDOWEVENT_RESIZED)
				SDL_GetWindowSize(game.window.window,
					&game.window.width, &game.window.height);
		}

		printf("%d ", game.window.width);
		printf("%d\n", game.window.height);

		glClearColor(game.window.color.r, game.window.color.g,
			game.window.color.b, game.window.color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_GL_SwapWindow(game.window.window);
	}

	SDL_GL_DeleteContext(game.window.context);
	SDL_DestroyWindow(game.window.window);
	SDL_Quit();
	return (0);
}
