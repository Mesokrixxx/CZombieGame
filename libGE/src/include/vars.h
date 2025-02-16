#ifndef GAMEENGINE_VARS_H
# define GAMEENGINE_VARS_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <SDL2/SDL.h>

# ifdef DEBUG
#  define _malloc(size)	malloc_debug(size)
#  define _free(ptr)	free_debug(ptr)

void	*malloc_debug(size_t size);
void	free_debug(void *ptr);

# else
#  define _malloc(size)	malloc(size)
#  define _free(ptr)	free(ptr)
# endif

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

typedef enum { false, true }		Bool;

typedef struct Vec2 { f32 x, y; }	Vec2;

#endif