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
	Velocity	v;
	Velocity	*v_ptr;

	v = (Velocity){0};
	v.maxSpeed = -1;
	v_ptr = &v;
	return (v_ptr);
}

void	*DefaultCircleSpriteCreator()
{
	CircleSprite	cb;
	CircleSprite	*cb_ptr;

	cb = (CircleSprite){0};
	cb.bodyColor = BLACK;
	cb.outlineColor = BLACK;
	cb_ptr = &cb;
	return (cb_ptr);
}
