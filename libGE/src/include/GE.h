#ifndef GAME_ENGINE_H
# define GAME_ENGINE_H

# include "GEVars.h"
# include "GETime.h"
# include "GEMath.h"
# include "GESparseSet.h"
# include "GEECS.h"
# include "GEEventBus.h"
# include "GEGraphics.h"

typedef struct GEInstance {
	SDL_Window		*window;
	SDL_GLContext	glContext;
	GEECS			*ecs;
	GEEventBus		*eventBus;
	GEGraphics		*graphics;

	Color			bgColor;
	iVec2			size;
	
	bool			debugMode;
	
	void			(*quitMethod)(struct GEInstance *);
	bool			running;
}	GEInstance;

// GEInstance
void	GECreateInstance(GEInstance *instance, char *title, iVec2 size, GEProjection projection);
void	GELaunchInstance(GEInstance *instance);
void	GEDestroyInstance(GEInstance *instance);

void	GESetBackgroundColor(GEInstance *instance, Color bgCol);
void	GESetQuitMethod(GEInstance *instance, void (*newQuitMethod)(GEInstance *instance));

#endif