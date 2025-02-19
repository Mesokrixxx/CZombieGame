#ifndef GAME_H
# define GAME_H

# include <GE.h>

# define WINDOW_WIDTH 1080
# define WINDOW_HEIGHT 720

typedef enum {
    OFFSET_CMP = DEFAULT_COMPONENT_COUNT,
    POSITION_CMP,
    VELOCITY_CMP,
	CIRCLE_SPRITE_CMP,
}   ComponentLocal;

typedef enum {
    OFFSET_EVTP = DEFAULT_EVENT_TYPE_COUNT,
}   EventTypeLocal;

typedef enum {
	OFFSET_VO = DEFAULT_VERTEX_OBJECT_COUNT,
}	VertexObjectLocal;

typedef struct {
	Vec2	vel;
	f32		acceleration;
	f32		deceleration;
	f32		maxSpeed;
}	Velocity;

void	InitGameContent();
void	LoadGameContent();

void	*DefaultVec2Creator();
void	*DefaultVelCreator();
void	*DefaultCircleSpriteCreator();

#endif