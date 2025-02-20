#include "game.h"

static void	updatePosSystem(u32 entityID, f32 dt);
static void	entitySprint(u32 entityID, f32 dt);
static void	drawCircleSprite(u32 entityID, f32 dt);

void	InitGameContent()
{
	// Composants
	RegisterComponent(POSITION_CMP, sizeof(Vec2), DefaultVec2Creator, NULL);
	RegisterComponent(VELOCITY_CMP, sizeof(VelParam), DefaultVelCreator, NULL);
	RegisterComponent(CIRCLE_SPRITE_CMP, sizeof(CircleSprite), DefaultCircleSpriteCreator, NULL);
	RegisterComponent(SPRINT_CMP, sizeof(SprintParam), DefaultSprintParamCreator, NULL);

	// Systems
	RegisterSystem(POSITION_CMP | VELOCITY_CMP, updatePosSystem);
	RegisterSystem(VELOCITY_CMP | SPRINT_CMP, entitySprint);
	RegisterSystem(POSITION_CMP | CIRCLE_SPRITE_CMP, drawCircleSprite);
}

static void	updatePosSystem(u32 entityID, f32 dt)
{
	Vec2		*entityPos = GetComponent(POSITION_CMP, entityID);
	VelParam	*entityVel = GetComponent(VELOCITY_CMP, entityID);

	if (entityVel->vel.x != 0 || entityVel->vel.y != 0)
	{
		f32 currentSpeed = LenghtVec2(entityVel->vel);
		
		// Cap speed
		if (currentSpeed > entityVel->currMaxSpeed && entityVel->currMaxSpeed > -1) // if -1 infinite speed
		{
			f32 scale = (entityVel->currMaxSpeed / currentSpeed);

			entityVel->vel = ScaleVec2(entityVel->vel, scale);
			currentSpeed *= scale;
		}

		// Decelerate
		if (currentSpeed > 0 && entityVel->currDecel > 0) // if currDecel equal to 0 then no deceleration
		{
			f32 decelAmount = entityVel->currAccel * dt;
			if (currentSpeed <= decelAmount)
				entityVel->vel = (Vec2){ 0 };
			else
				entityVel->vel = ScaleVec2(entityVel->vel, (currentSpeed - decelAmount) / currentSpeed);
		}

		// Update Pos
		*entityPos = AddVec2s(*entityPos, ScaleVec2(entityVel->vel, dt));
	}

	if (entityVel->currAccel != entityVel->acceleration
		|| entityVel->currDecel != entityVel->deceleration
		|| entityVel->currMaxSpeed != entityVel->maxSpeed)
	{
		entityVel->currAccel = entityVel->acceleration;
		entityVel->currDecel = entityVel->deceleration;
		entityVel->currMaxSpeed = entityVel->maxSpeed;
	}
}

static void	entitySprint(u32 entityID, f32 dt)
{
	VelParam	*entityVel = GetComponent(VELOCITY_CMP, entityID);
	SprintParam	*entitySprint = GetComponent(SPRINT_CMP, entityID);

	if (entitySprint->sprinting)
	{
		entityVel->currAccel = entitySprint->sprintAccel;
		entityVel->currDecel = entitySprint->sprintDecel;
		entityVel->currMaxSpeed = entitySprint->sprintSpeed;
		entitySprint->sprinting = false;
	}

	(void)dt;
}

static void	drawCircleSprite(u32 entityID, f32 dt)
{
	Vec2			*entityPos = GetComponent(POSITION_CMP, entityID);
	CircleSprite	*bodyParams = GetComponent(CIRCLE_SPRITE_CMP, entityID);
	
	(void)dt;
	DrawCircle(*entityPos, *bodyParams);
}
