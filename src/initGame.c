#include "game.h"

static void	updatePosSystem(u32 entityID, f32 dt);
static void	drawCircleSprite(u32 entityID, f32 dt);

void	InitGameContent()
{
	// Composants
	RegisterComponent(POSITION_CMP, sizeof(Vec2), DefaultVec2Creator, NULL);
	RegisterComponent(VELOCITY_CMP, sizeof(Velocity), DefaultVelCreator, NULL);
	RegisterComponent(CIRCLE_SPRITE_CMP, sizeof(CircleSprite), DefaultCircleSpriteCreator, NULL);

	// Systems
	RegisterSystem(POSITION_CMP | VELOCITY_CMP, updatePosSystem);
	RegisterSystem(POSITION_CMP | CIRCLE_SPRITE_CMP, drawCircleSprite);
}

static void	updatePosSystem(u32 entityID, f32 dt)
{
	Vec2		*entityPos = GetComponent(POSITION_CMP, entityID);
	Velocity	*entityVel = GetComponent(VELOCITY_CMP, entityID);

	if (entityVel->vel.x != 0 && entityVel->vel.y != 0)
	{
		f32 currentSpeed = LenghtVec2(entityVel->vel);
		
		// Cap speed
		if (currentSpeed > entityVel->maxSpeed && entityVel->maxSpeed > -1) // if -1 infinite speed
		{
			f32 scale = (entityVel->maxSpeed / currentSpeed);

			entityVel->vel = ScaleVec2(entityVel->vel, scale);
			currentSpeed *= scale;
		}

		// Decelerate
		if (entityVel->deceleration > 1)
			entityVel->deceleration = 1;
		if (currentSpeed > 0 && entityVel->deceleration > 0) // if deceleration equal to 0 then no deceleration
		{
			f32 scale = 1 - entityVel->deceleration * dt;
			entityVel->vel = ScaleVec2(entityVel->vel, scale > 0 ? scale : 0);
		}

		// Update Pos
		*entityPos = AddVec2s(*entityPos, ScaleVec2(entityVel->vel, dt));
	}
}

static void	drawCircleSprite(u32 entityID, f32 dt)
{
	Vec2			*entityPos = GetComponent(POSITION_CMP, entityID);
	CircleSprite	*bodyParams = GetComponent(CIRCLE_SPRITE_CMP, entityID);
	
	(void)dt;
	DrawCircle(SHADERPROG_POSNCOLOR_DEFAULT, entityPos, bodyParams);
}
