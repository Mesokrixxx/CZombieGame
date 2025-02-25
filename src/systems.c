#include "zombieGame.h"

void MovementSystem(void *vinstance, u32 entityID, f32 dt)
{
	GEInstance	*instance = vinstance;
	Vec2	*entityPos = GEGetComponent(instance->ecs, POS_COMP, entityID);
	VelComp	*entityVel = GEGetComponent(instance->ecs, VEL_COMP, entityID);

	if (entityVel->velocity.x != 0 && entityVel->velocity.y != 0)
	{
		f32 currentSpeed = LenghtVec2(entityVel->velocity);
		
		// Cap speed
		if (currentSpeed > entityVel->maxSpeed && entityVel->maxSpeed > -1) // if -1 infinite speed
		{
			f32 scale = (entityVel->maxSpeed / currentSpeed);

			entityVel->velocity = ScaleVec2(entityVel->velocity, scale);
			currentSpeed *= scale;
		}

		// Decelerate
		if (entityVel->deceleration > 1)
			entityVel->deceleration = 1;
		if (currentSpeed > 0 && entityVel->deceleration > 0) // if deceleration equal to 0 then no deceleration
		{
			f32 scale = 1 - entityVel->deceleration * dt;
			entityVel->velocity = ScaleVec2(entityVel->velocity, scale > 0 ? scale : 0);
		}

		// Update Pos
		*entityPos = AddVec2s(*entityPos, ScaleVec2(entityVel->velocity, dt));
	}
}

void RenderSystem(void *vinstance, u32 entityID, f32 dt)
{
	GEInstance	*instance = vinstance;
	
	if (*(u32 *)GEGetFlags(instance->ecs, entityID) & CIRCLE_COMP)
	{
		Vec2		*entityPos = GEGetComponent(instance->ecs, POS_COMP, entityID);
		CircleComp	*entityCircle = GEGetComponent(instance->ecs, CIRCLE_COMP, entityID);

		GLuint shaderProg = GEUseShader(instance->graphics, SHADERPROG_CIRCLE);
		GEUseVAO(instance->graphics, VO_CIRCLE);

		GLint modelLoc = glGetUniformLocation(shaderProg, "model");
		GLint colorLoc = glGetUniformLocation(shaderProg, "color");
		GLint outlineSize = glGetUniformLocation(shaderProg, "outlineSize");
		GLint outlineColor = glGetUniformLocation(shaderProg, "outlineColor");
		f32	matrice[16];

		Mat4x4	model = Mat4x4Identity();
		TranslateMat4x4(&model, Vec3FromVec2(*entityPos, 0));
		ScaleMat4x4(&model, newVec3(entityCircle->radius, entityCircle->radius, 1));
		GEMat4x4ToFloat(matrice, model);

		glUniformMatrix4fv(modelLoc,
			1, GL_FALSE, matrice);
		glUniform4f(colorLoc,
			entityCircle->color.r, entityCircle->color.g,
			entityCircle->color.b, entityCircle->color.a);
		glUniform1f(outlineSize,
			entityCircle->outlineSize);
		glUniform4f(outlineColor,
			entityCircle->outlineColor.r, entityCircle->outlineColor.g,
			entityCircle->outlineColor.b, entityCircle->outlineColor.a);

		glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_SEGMENTS);
	}
}
