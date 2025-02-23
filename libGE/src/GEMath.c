#include "GEPrivate.h"

static Mat4x4	_createOrthoProjMatrice(iVec2 size);

Vec2	newVec2(f32 x, f32 y) { return ((Vec2){ x, y }); }
iVec2	newIVec2(i32 x, i32 y) { return ((iVec2){ x, y }); }
Vec3	newVec3(f32 x, f32 y, f32 z) { return ((Vec3){ x, y, z }); }
iVec3	newIVec3(i32 x, i32 y, i32 z) { return ((iVec3){ x, y, z }); }
Vec4	newVec4(f32 x, f32 y, f32 z, f32 w) { return ((Vec4){ x, y, z, w }); }
iVec4	newIVec4(i32 x, i32 y, i32 z, i32 w) { return ((iVec4){ x, y, z, w }); }

f32		LenghtVec2Squared(Vec2 v)
{
	return (v.x * v.x + v.y * v.y);
}

f32		LenghtVec2(Vec2 v)
{
	return (SDL_sqrtf(v.x * v.x + v.y * v.y));
}

Vec2	ScaleVec2(Vec2 v, f32 scale)
{
	return (newVec2(v.x * scale, v.y * scale));
}

Vec2	AddVec2s(Vec2 v1, Vec2 v2)
{
	return (newVec2(v1.x + v2.x, v1.y + v2.y));
}

Vec3	Vec3FromVec2(Vec2 v, f32 z)
{
	return (newVec3(v.x, v.y, z));
}

Vec4	Vec4FromVec2(Vec2 v, f32 z, f32 w)
{
	return (newVec4(v.x, v.y, z, w));
}

Vec4	Vec4FromVec3(Vec3 v, f32 w)
{
	return (newVec4(v.x, v.y, v.z, w));
}

Mat4x4	Mat4x4Identity()
{
	return ((Mat4x4){ 
		.a = (Vec4) { 1, 0, 0, 0 },
		.b = (Vec4) { 0, 1, 0, 0 },
		.c = (Vec4) { 0, 0, 1, 0 },
		.d = (Vec4) { 0, 0, 0, 1 }
	});
}

Mat4x4	Mat4x4Translation(Vec3 v)
{
	return ((Mat4x4){ 
		.a = (Vec4) { 1, 0, 0, 0 },
		.b = (Vec4) { 0, 1, 0, 0 },
		.c = (Vec4) { 0, 0, 1, 0 },
		.d = (Vec4) { v.x, v.y, v.z, 1 }
	});
}

void	TranslateMat4x4(Mat4x4 *m, Vec3 v)
{
	m->d.x += v.x;
	m->d.y += v.y;
	m->d.z += v.z;
}

void	ScaleMat4x4(Mat4x4 *m, Vec3 v)
{
	m->a.x *= v.x;
	m->b.y *= v.y;
	m->c.z *= v.z;
}

Mat4x4	GECreateProjectionMatrice(iVec2 size, GEProjection proj)
{
	if (proj == GE_PROJECTION_ORTHO)
		return (_createOrthoProjMatrice(size));

	return ((Mat4x4){ 0 });
}

void	GEMat4x4ToFloat(f32 *f, Mat4x4 m)
{
	f[0] = m.a.x; f[1] = m.a.y; f[2] = m.a.z; f[3] = m.a.w;
	f[4] = m.b.x; f[5] = m.b.y; f[6] = m.b.z; f[7] = m.b.w;
	f[8] = m.c.x; f[9] = m.c.y; f[10] = m.c.z; f[11] = m.c.w;
	f[12] = m.d.x; f[13] = m.d.y; f[14] = m.d.z; f[15] = m.d.w;	
}

static Mat4x4	_createOrthoProjMatrice(iVec2 size)
{
	Mat4x4	m;

	f32	left = 0.0f, right = size.x;
	f32	top = 0.0f, bottom = size.y;
	f32 nearP = -1.0f, farP = 1.0f;

	m = (Mat4x4){ 
		.a = newVec4(2 / (right - left), 0, 0, 0),
		.b = newVec4(0, 2 / (top - bottom), 0, 0),
		.c = newVec4(0, 0, -2 / (farP - nearP), 0),
		.d = newVec4(-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(farP + nearP) / (farP - nearP), 1)
	};
	return (m);
}
