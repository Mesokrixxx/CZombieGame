# include "game.h"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	if (!CreateInstance("Zombie Game", WINDOW_WIDTH, WINDOW_HEIGHT, PROJ_ORTHO)) {
		printf("Failed to create engine\n");
		return (1);
	}

	InitGameContent();
	LoadGameContent();

	SetInstanceBGCol(LIGHT_GRAY);

	LaunchInstance();
	DestroyInstance();

	return (0);	
}
