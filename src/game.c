#include "game.h"

static void	*defaultVec2Creator();

void	initGameComponent()
{
	RegisterComponent(POSITION_CMP, sizeof(Vec2), defaultVec2Creator, NULL);
	RegisterComponent(VELOCITY_CMP, sizeof(Vec2), defaultVec2Creator, NULL);
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	if (!CreateInstance("Zombie Game", 1080, 720)) {
		printf("Failed to create engine\n");
		return (1);
	}

	initGameComponent();

	LaunchInstance();
	DestroyInstance();
	return (0);	
}

static void	*defaultVec2Creator()
{
	Vec2 v;
	Vec2 *v_ptr; 
	
	v = (Vec2){0};
	v_ptr = &v;
	return (v_ptr);
}
