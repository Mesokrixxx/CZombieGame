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
Vec4	newVec4(f32 x, f32 y, f32 z, f32 w);
iVec4	newIVec4(i32 x, i32 y, i32 z, i32 w);

f32		LenghtVec2Squared(Vec2 v);
f32		LenghtVec2(Vec2 v);
Vec2	ScaleVec2(Vec2 v, f32 scale);
Vec2	AddVec2s(Vec2 v1, Vec2 v2);
Vec3	Vec3FromVec2(Vec2 v, f32 z);
Vec4	Vec4FromVec2(Vec2 v, f32 z, f32 w);
Vec4	Vec4FromVec3(Vec3 v, f32 w);
Mat4x4	Mat4x4Identity();
Mat4x4	Mat4x4Translation(Vec3 v);
void	TranslateMat4x4(Mat4x4 *m, Vec3 v);
void	ScaleMat4x4(Mat4x4 *m, Vec3 v);

Mat4x4	GECreateProjectionMatrice(iVec2 size, GEProjection proj);
void	GEMat4x4ToFloat(f32 *f, Mat4x4 m);

#endif