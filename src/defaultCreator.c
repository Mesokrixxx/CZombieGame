#include "game.h"

void	*DefaultVec2Creator()
{
	Vec2 v;
	Vec2 *v_ptr; 
	
	v = (Vec2){0};
	v_ptr = &v;
	return (v_ptr);
}

void	*DefaultVelCreator()
{
	VelParam	v;
	VelParam	*v_ptr;

	v = (VelParam){0};
	v.maxSpeed = -1;
	v_ptr = &v;
	return (v_ptr);
}

void	*DefaultSprintParamCreator()
{
	SprintParam	sp;	
	SprintParam	*sp_ptr;
	
	sp = (SprintParam){ 0 };
	sp_ptr = &sp;
	return (sp_ptr);
}

void	*DefaultCircleSpriteCreator()
{
	CircleSprite	cb;
	CircleSprite	*cb_ptr;

	cb = (CircleSprite){0};
	cb.color = BLACK;
	cb.outlineColor = BLACK;
	cb_ptr = &cb;
	return (cb_ptr);
}
