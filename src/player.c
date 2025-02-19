#include "game.h"

u32	playerID;

static void	movePlayer(void *data);

void	LoadNewPlayer()
{
	playerID = CreateEntity(DEFAULT_ENTITY | SPRINT_CMP);

	Vec2			*pos = GetComponent(POSITION_CMP, playerID);
	*pos = (Vec2){WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
	
	VelParam		*vel = GetComponent(VELOCITY_CMP, playerID);
	vel->acceleration = 150.0f;
	vel->deceleration = vel->acceleration * 1.2;
	vel->maxSpeed = 120;
	
	CircleSprite	*circleSprite = GetComponent(CIRCLE_SPRITE_CMP, playerID);
	circleSprite->color = DARK_GRAY;
	circleSprite->radius = 20;
	circleSprite->outlineSize = 0.15f;

	SprintParam		*sprint = GetComponent(SPRINT_CMP, playerID);
	sprint->sprintAccel = vel->acceleration * 1.5f;
	sprint->sprintDecel = vel->deceleration;
	sprint->sprintSpeed = vel->maxSpeed * 1.8f;

	NewEventListener(KEYDOWN_EVTP, movePlayer);
}

static void	movePlayer(void *data)
{
	SDL_Scancode	scancode = *(SDL_Scancode *)data;
	VelParam		*playerVel = GetComponent(VELOCITY_CMP, playerID);
	SprintParam		*playerSprint = GetComponent(SPRINT_CMP, playerID);

	if (scancode == SDL_SCANCODE_W)
		playerVel->vel.y -= playerVel->currAccel;
	else if (scancode == SDL_SCANCODE_S)
		playerVel->vel.y += playerVel->currAccel;
	
	if (scancode == SDL_SCANCODE_D)
		playerVel->vel.x += playerVel->currAccel;
	else if (scancode == SDL_SCANCODE_A)
		playerVel->vel.x -= playerVel->currAccel;

	if (scancode == SDL_SCANCODE_LSHIFT)
		playerSprint->sprinting = true;
}
