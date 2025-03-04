#ifndef ZOMBIEGAME_VARS_H
# define ZOMBIEGAME_VARS_H

# include <stdlib.h>
# include <stdio.h>
# include <stdint.h>

typedef	int8_t		i8;
typedef	int16_t		i16;
typedef	int32_t		i32;
typedef	int64_t		i64;
typedef	uint8_t		u8;
typedef	uint16_t	u16;
typedef	uint32_t	u32;
typedef	uint64_t	u64;
typedef	float		f32;
typedef	double		f64;

typedef enum { false, true }		bool;
typedef struct { f32 r, g, b, a; }	color;

# define WHITE	(color){ 1, 1, 1, 1 }
# define BLACK	(color){ 0, 0, 0, 1 }
# define RED	(color){ 1, 0, 0, 1 }
# define GREEN	(color){ 0, 1, 0, 1 }
# define BLUE	(color){ 0, 0, 1, 1 }

# define DEFAULT_WINDOW_WIDTH	1080
# define DEFAULT_WINDOW_HEIGHT	720

# define ASSERT(_c, ...) if (!(_c)) { fprintf(stderr, __VA_ARGS__); exit(1); }
# define _malloc(size)					debug_malloc(size)
# define _free(ptr)						debug_free(ptr)
# define _realloc(ptr, oldsize, size)	debug_realloc(ptr, oldsize, size)

void	*debug_malloc(size_t size);
void	debug_free(void *ptr);
void	*debug_realloc(void *ptr, size_t oldsize, size_t size);

#endif