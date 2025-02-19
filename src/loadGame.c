#include "game.h"

static void	LoadNewPlayer();

void	LoadGameContent()
{
	LoadNewPlayer();
}

static void	LoadNewPlayer()
{
	u32	playerID;

	playerID = CreateEntity(POSITION_CMP | VELOCITY_CMP | CIRCLE_SPRITE_CMP);

	Vec2			*pos = GetComponent(POSITION_CMP, playerID);
	*pos = (Vec2){WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
	
	Velocity		*vel = GetComponent(VELOCITY_CMP, playerID);
	vel->acceleration = 15.0f;
	vel->deceleration = 0.2f;
	vel->maxSpeed = 20;
	
	CircleSprite	*circleSprite = GetComponent(CIRCLE_SPRITE_CMP, playerID);
	circleSprite->color = DARK_GRAY;
	circleSprite->radius = 20;
}
