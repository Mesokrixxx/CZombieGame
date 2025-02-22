#ifndef GAME_ENGINE_VARS_H
# define GAME_ENGINE_VARS_H

// ECS
# define GE_ENTITY_CHUNK_SIZE 256
# define GE_SYSTEM_CHUNK_SIZE 48
# define GE_COMPS_CHUNK_SIZE 24

// EVENTBUS
# define GE_EVENT_LISTENERS_CHUNK_SIZE 24
# define GE_EVENTTYPE_CHUNK_SIZE 12

// GRAPHICS
# define GE_VOS_CHUNK_SIZE 24
# define GE_SHADERPROGS_CHUNK_SIZE 24

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <GL/glew.h>
# include <SDL2/SDL.h>

typedef float		f32;
typedef double		f64;
	
typedef int8_t		i8;
typedef int16_t		i16;
typedef int32_t		i32;
typedef int64_t		i64;

typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

typedef enum { false, true }		bool;
typedef struct { u8 r, g, b, a; }	Color;

typedef struct { f32 x, y; }		Vec2;
typedef struct { i32 x, y; }		iVec2;
typedef struct { f32 x, y, z; }		Vec3;
typedef struct { i32 x, y, z; }		iVec3;

# define GE_COLOR_WHITE	(Color){ 1.0, 1.0, 1.0, 1.0 };
# define GE_COLOR_BLACK	(Color){ 0.0, 0.0, 0.0, 1.0 };
# define GE_COLOR_RED	(Color){ 1.0, 0.0, 0.0, 1.0 };
# define GE_COLOR_BLUE	(Color){ 0.0, 1.0, 0.0, 1.0 };
# define GE_COLOR_GREEN	(Color){ 0.0, 0.0, 1.0, 1.0 };

typedef struct {
	u8		button;
	iVec2	pos;
}	MouseEvent;

#endif