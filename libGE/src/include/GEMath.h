#ifndef GAME_ENGINE_MATH_H
# define GAME_ENGINE_MATH_H

# include "GEVars.h"

# define GE_PI 3.1415926f

typedef enum {
	GE_PROJECTION_ORTHO,
	GE_PROJECTION_PERSEPECTIVE,
}	GEProjection;

Vec2	newVec2(f32 x, f32 y);
iVec2	newIVec2(i32 x, i32 y);
Vec3	newVec3(f32 x, f32 y, f32 z);
iVec3	newIVec3(i32 x, i32 y, i32 z);

Mat4x4	GECreateProjectionMatrice(iVec2 size, GEProjection proj);
void	GEMat4x4ToFloat(f32 *f, Mat4x4 m);

#endif