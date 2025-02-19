#include "dtime.h"

extern f32	AimedFPS;
f32	deltaTime = 0;

void	UpdateDeltaTimeNCapFPS()
{
	static u64 lastTime = 0;
	u64 currentTime = SDL_GetPerformanceCounter();
	f64 frequency = (f64)SDL_GetPerformanceFrequency();

	if (lastTime == 0) {
		lastTime = currentTime;
		deltaTime = 1.0f / 60.0f;
		return;
	}

	deltaTime = (f32)((currentTime - lastTime) / frequency);

	if (AimedFPS > 0) // if 0 no Cap
	{
		f32 targetFrameTime = 1.0f / AimedFPS;
		if (deltaTime < targetFrameTime)
		{
			f32 remainingTimeMs = (targetFrameTime - deltaTime) * 1000.0f;

			if (remainingTimeMs > 1.0f)
				SDL_Delay((Uint32)(remainingTimeMs - 1));

			while (((SDL_GetPerformanceCounter() - lastTime) / frequency) < targetFrameTime)
				;

			currentTime = SDL_GetPerformanceCounter();
			deltaTime = (f32)((currentTime - lastTime) / frequency);
		}
	}
	lastTime = currentTime;
}

f32	GetDeltaTime()
{
	return (deltaTime);
}
