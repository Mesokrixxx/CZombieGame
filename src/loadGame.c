#include "game.h"

static void	LoadNewPlayer();
static void	movePlayer(void *data);

u32	playerID;

void	LoadGameContent()
{
	LoadNewPlayer();
}

static void	LoadNewPlayer()
{
	playerID = CreateEntity(POSITION_CMP | VELOCITY_CMP | CIRCLE_SPRITE_CMP);

	Vec2			*pos = GetComponent(POSITION_CMP, playerID);
	*pos = (Vec2){WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
	
	Velocity		*vel = GetComponent(VELOCITY_CMP, playerID);
	vel->acceleration = 150.0f;
	vel->deceleration = vel->acceleration * 1.2;
	vel->maxSpeed = 120;
	
	CircleSprite	*circleSprite = GetComponent(CIRCLE_SPRITE_CMP, playerID);
	circleSprite->color = DARK_GRAY;
	circleSprite->radius = 20;
	circleSprite->outlineSize = 0.15f;

	NewEventListener(KEYDOWN_EVTP, movePlayer);
}

static void	movePlayer(void *data)
{
	SDL_Scancode	scancode = *(SDL_Scancode *)data;
	Velocity		*playerVel = GetComponent(VELOCITY_CMP, playerID);

	if (scancode == SDL_SCANCODE_W)
		playerVel->vel.y -= playerVel->acceleration;
	else if (scancode == SDL_SCANCODE_S)
		playerVel->vel.y += playerVel->acceleration;
	
	if (scancode == SDL_SCANCODE_D)
		playerVel->vel.x += playerVel->acceleration;
	else if (scancode == SDL_SCANCODE_A)
		playerVel->vel.x -= playerVel->acceleration;
}
