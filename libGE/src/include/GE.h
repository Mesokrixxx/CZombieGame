#ifndef GAME_ENGINE_H
# define GAME_ENGINE_H

# include "GEVars.h"
# include "GEMath.h"
# include "GESparseSet.h"
# include "GEECS.h"

typedef struct {
	SDL_Window		*window;
	SDL_GLContext	glContext;
	GEECS			*ecs;
	Color			bgColor;
	iVec2			size;
	bool			running;
}	GEInstance;

// GEInstance
void	GECreateInstance(GEInstance *instance, char *title, iVec2 size, GEProjection projection);
void	GELaunchInstance(GEInstance *instance);
void	GEDestroyInstance(GEInstance *instance);

#endif