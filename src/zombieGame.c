#include "zombieGame.h"

int main(int argc, char **argv)
{
	GEInstance	*mainInstance;
	
	mainInstance = malloc(sizeof(GEInstance));
	if (!mainInstance)
	{
		printf("Failed to allocate for mainInstance\n");
		return (1);
	}

	GECreateInstance(mainInstance, "Zombie Game", newIVec2(WINDOW_WIDTH, WINDOW_HEIGHT), GE_PROJECTION_ORTHO);

	LoadContent(mainInstance);

	u32 entityTest = GECreateEntity(mainInstance->ecs, POS_COMP | VEL_COMP);

	GELaunchInstance(mainInstance);
	GEDestroyInstance(mainInstance);

	free(mainInstance);

	return (0);	
}
