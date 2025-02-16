#include "dtime.h"

f32	deltaTime = 0;

void	UpdateDeltaTime()
{
	static u64	lastTime = 0;
	u64			currentTime;
	
	currentTime = SDL_GetTicks();
	
	if (lastTime == 0)
	{
		lastTime = currentTime;
		deltaTime = 1.0f / 60.0f;
		return;
	}
	
	deltaTime = (f32)(currentTime - lastTime) / 1000.0f;
	lastTime = currentTime;
}

f32	GetDeltaTime()
{
	return (deltaTime);
}
