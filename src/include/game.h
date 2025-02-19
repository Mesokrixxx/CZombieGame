#ifndef GAME_H
# define GAME_H

# include <GE.h>

# define WINDOW_WIDTH 1080
# define WINDOW_HEIGHT 720

typedef enum {
    OFFSET_CMP = DEFAULT_COMPONENT_COUNT,
    POSITION_CMP,
    VELOCITY_CMP,
	SPRINT_CMP,
	CIRCLE_SPRITE_CMP,
}   ComponentLocal;

typedef enum {
    OFFSET_EVTP = DEFAULT_EVENT_TYPE_COUNT,
}   EventTypeLocal;

typedef enum {
	OFFSET_VO = DEFAULT_VERTEX_OBJECT_COUNT,
}	VertexObjectLocal;

typedef enum {
	OFFSET_SHPROG = DEFAULT_SHADER_PROGRAM_COUNT,
}	ShaderProgramLocal;

# define DEFAULT_ENTITY (POSITION_CMP | VELOCITY_CMP | CIRCLE_SPRITE_CMP)

typedef struct {
	Vec2	vel;
	f32		acceleration;
	f32		currAccel;
	f32		deceleration;
	f32		currDecel;
	f32		maxSpeed;
	f32		currMaxSpeed;
}	VelParam;

typedef struct {
	f32		sprintAccel;
	f32		sprintDecel;
	f32		sprintSpeed;
	Bool	sprinting;
}	SprintParam;

void	InitGameContent();
void	LoadGameContent();

void	*DefaultVec2Creator();
void	*DefaultVelCreator();
void	*DefaultSprintParamCreator();
void	*DefaultCircleSpriteCreator();

void	LoadNewPlayer();

#endif