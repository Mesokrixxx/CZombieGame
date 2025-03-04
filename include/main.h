#ifndef ZOMBIEGAME_MAIN_H
# define ZOMBIEGAME_MAIN_H

# include "vars.h"
# include <SDL2/SDL.h>
# include <GL/glew.h>

typedef struct {
	struct {
		SDL_Window		*window;
		SDL_GLContext	context;
		color			color;
		i32				width;
		i32				height;
	}		window;

	bool	running;
}	ZombieGame;

#endif