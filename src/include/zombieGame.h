#ifndef ZOMBIEGAME_H
# define ZOMBIEGAME_H

# include <GE.h>

# define CIRCLE_SEGMENTS 32

typedef enum {
	SHADERPROG_CIRCLE,
}	ShaderProgramIDs;

typedef enum {
	VO_CIRCLE,
}	VOIDs;

typedef struct {
	Vec2	velocity;
	f32		acceleration;
	f32		deceleration;
	f32		maxSpeed;
}	VelComp;

typedef struct {
	f32		radius, outlineSize;
	Color	color, outlineColor;
}	CircleComp;

typedef enum {
	OFFSET = GE_COMP_COUNT,
	POS_COMP,
	VEL_COMP,
	SPRITE_COMP,
	_CIRCLE,
}	LocalComponent;

# define CIRCLE_COMP (SPRITE_COMP | _CIRCLE)

# define WINDOW_WIDTH 1080
# define WINDOW_HEIGHT 720

// Load Content
void	LoadContent(GEInstance *instance);
void	MovementSystem(void *, u32, f32);
void	RenderSystem(void *, u32, f32);

// Init Content
void	InitContent(GEInstance *instance);
void	NewPlayer(GEInstance *);

#endif
