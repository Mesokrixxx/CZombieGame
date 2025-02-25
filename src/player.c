#include "zombieGame.h"

static void	_keyDownListenerPlayer(void *vinstance, void *data, u32 playerID);

void	NewPlayer(GEInstance *instance)
{
	u32			playerID; 
	Vec2		*playerPos;
	VelComp		*playerVel;
	CircleComp	*playerSprite;

	playerID = GECreateEntity(instance->ecs, CIRCLE_COMP | POS_COMP | VEL_COMP);
	playerPos = GEGetComponent(instance->ecs, POS_COMP, playerID);
	playerVel = GEGetComponent(instance->ecs, VEL_COMP, playerID);
	playerSprite = GEGetComponent(instance->ecs, CIRCLE_COMP, playerID);

	*playerPos = newVec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	playerVel->acceleration = 50;
	playerVel->deceleration = 100;
	playerVel->maxSpeed = 200;

	playerSprite->radius = 16;
	playerSprite->color = GE_COLOR_LIGHT_GRAY;
	playerSprite->outlineSize = 2;

	GEAddEventListener(instance->eventBus, GE_EVENT_TYPE_KEYDOWN, GECreateEventListener(
		_keyDownListenerPlayer, playerID));
}

static void	_keyDownListenerPlayer(void *vinstance, void *data, u32 playerID)
{
	SDL_Scancode code = *(SDL_Scancode *)data;

	GEInstance	*instance = vinstance;
	VelComp		*vel = GEGetComponent(instance->ecs, VEL_COMP, playerID);

	if (code == SDL_SCANCODE_W)
		vel->velocity.y += -vel->acceleration;
	else if (code == SDL_SCANCODE_S)
		vel->velocity.y += vel->acceleration;
	
	if (code == SDL_SCANCODE_A)
		vel->velocity.x += -vel->acceleration;
	else if (code == SDL_SCANCODE_D)
		vel->velocity.x += vel->acceleration;
}
