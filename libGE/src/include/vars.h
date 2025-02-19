#ifndef GAMEENGINE_VARS_H
# define GAMEENGINE_VARS_H

/* Détection de la plateforme */
# if defined(_WIN32) || defined(_WIN64)
#  define GE_WINDOWS
#  include <windows.h>
# elif defined(__APPLE__)
#  define GE_MACOS
# elif defined(__linux__)
#  define GE_LINUX
# endif

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <SDL2/SDL.h>
# include <GL/glew.h>

# ifdef DEBUG
#  define _malloc(size)	malloc_debug(size)
#  define _free(ptr)	free_debug(ptr)

void	*malloc_debug(size_t size);
void	free_debug(void *ptr);

# else
#  define _malloc(size)	malloc(size)
#  define _free(ptr)	free(ptr)
# endif

# define PI	3.1415926f

# define LOG(...) printf(__VA_ARGS__)
# define ASSERT(_c, ...) if (!(_c)) { fprintf(stderr, __VA_ARGS__); exit(1); }

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

typedef enum { false, true }				Bool;
typedef struct Vec2 { f32	x, y; }			Vec2;
typedef struct iVec2 { i32	x, y; }			iVec2;
typedef struct Vec3 { f32 x, y, z; }		Vec3;
typedef struct Vec4 { f32 x, y, z, w; }		Vec4;
typedef struct Mat4x4 { Vec4 a, b, c, d; }	Mat4x4;

typedef struct Color { f32	r, g, b, a; }	Color;

#define BLACK      (Color){ 0.0f, 0.0f, 0.0f, 1.0f }
#define WHITE      (Color){ 1.0f, 1.0f, 1.0f, 1.0f }
#define RED        (Color){ 1.0f, 0.0f, 0.0f, 1.0f }
#define GREEN      (Color){ 0.0f, 1.0f, 0.0f, 1.0f }
#define BLUE       (Color){ 0.0f, 0.0f, 1.0f, 1.0f }
#define YELLOW     (Color){ 1.0f, 1.0f, 0.0f, 1.0f }
#define CYAN       (Color){ 0.0f, 1.0f, 1.0f, 1.0f }
#define MAGENTA    (Color){ 1.0f, 0.0f, 1.0f, 1.0f }
#define ORANGE     (Color){ 1.0f, 0.647f, 0.0f, 1.0f }
#define PURPLE     (Color){ 0.5f, 0.0f, 0.5f, 1.0f }
#define BROWN      (Color){ 0.647f, 0.165f, 0.165f, 1.0f }
#define PINK       (Color){ 1.0f, 0.753f, 0.796f, 1.0f }
#define GRAY       (Color){ 0.5f, 0.5f, 0.5f, 1.0f }
#define LIGHT_GRAY (Color){ 0.827f, 0.827f, 0.827f, 1.0f }
#define DARK_GRAY  (Color){ 0.663f, 0.663f, 0.663f, 1.0f }
#define GOLD       (Color){ 1.0f, 0.843f, 0.0f, 1.0f }
#define SILVER     (Color){ 0.753f, 0.753f, 0.753f, 1.0f }
#define TEAL       (Color){ 0.0f, 0.5f, 0.5f, 1.0f }
#define NAVY       (Color){ 0.0f, 0.0f, 0.5f, 1.0f }
#define MAROON     (Color){ 0.5f, 0.0f, 0.0f, 1.0f }
#define OLIVE      (Color){ 0.5f, 0.5f, 0.0f, 1.0f }
#define CHOCOLATE  (Color){ 0.824f, 0.412f, 0.118f, 1.0f }
#define CORAL      (Color){ 1.0f, 0.498f, 0.314f, 1.0f }
#define VIOLET     (Color){ 0.933f, 0.51f, 0.933f, 1.0f }
#define INDIGO     (Color){ 0.294f, 0.0f, 0.509f, 1.0f }
#define LAVENDER   (Color){ 0.902f, 0.902f, 0.98f, 1.0f }
#define SALMON     (Color){ 0.98f, 0.502f, 0.447f, 1.0f }
#define TURQUOISE  (Color){ 0.251f, 0.878f, 0.816f, 1.0f }
#define BEIGE      (Color){ 0.961f, 0.961f, 0.863f, 1.0f }
#define CHARTREUSE (Color){ 0.498f, 1.0f, 0.0f, 1.0f }
#define CRIMSON    (Color){ 0.863f, 0.078f, 0.235f, 1.0f }
#define PERU       (Color){ 0.804f, 0.522f, 0.247f, 1.0f }

typedef struct {
	f32		radius;
	f32		outlineSize;
	Color	bodyColor;
	Color	outlineColor;
}	CircleSprite;

typedef enum {
	PROJ_ORTHO,
}	ProjType;

#endif