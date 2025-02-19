#include "GE.h"

extern Instance	*instance;

f32	LenghtVec2Squared(Vec2 v)
{
	return (v.x * v.x + v.y * v.y);
}

f32	LenghtVec2(Vec2 v)
{
	return (SDL_sqrtf(v.x * v.x + v.y * v.y));
}

Vec2	ScaleVec2(Vec2 v, f32 scale)
{
	return ((Vec2){ v.x * scale, v.y * scale });
}

Vec2	AddVec2s(Vec2 v1, Vec2 v2)
{
	return ((Vec2){ v1.x + v2.x, v1.y + v2.y });
}

Vec3	Vec3FromVec2(Vec2 v, f32 z)
{
	return ((Vec3){ v.x, v.y, z });
}

Vec4	Vec4FromVec2(Vec2 v, f32 z, f32 w)
{
	return ((Vec4){ v.x, v.y, z, w });
}

Vec4	Vec4FromVec3(Vec3 v, f32 w)
{
	return ((Vec4){ v.x, v.y, v.z, w });
}


void	Mat4x4ToFloat(Mat4x4 m, f32 *f)
{	
	f[0] = m.a.x; f[1] = m.a.y; f[2] = m.a.z; f[3] = m.a.w;
	f[4] = m.b.x; f[5] = m.b.y; f[6] = m.b.z; f[7] = m.b.w;
	f[8] = m.c.x; f[9] = m.c.y; f[10] = m.c.z; f[11] = m.c.w;
	f[12] = m.d.x; f[13] = m.d.y; f[14] = m.d.z; f[15] = m.d.w;
}

Mat4x4	CreateOrthoProjMatrice()
{
	Mat4x4	m;

	f32	right = 0.0f, left = instance->windowParam.w;
	f32	top = 0.0f, bottom = instance->windowParam.h;
	f32 nearP = -1.0f, farP = 1.0f;

	m = (Mat4x4){ 
		.a = (Vec4) { 2 / (right - left), 0, 0, 0 },
		.b = (Vec4) { 0, 2 / (top - bottom), 0, 0 },
		.c = (Vec4) { 0, 0, 2 / (farP - nearP), 0 },
		.d = (Vec4) { -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(farP + nearP) / (farP - nearP), 1 }
	};

	return (m);
}

Mat4x4	CreateProjectionMatrice(ProjType pt)
{
	if (pt == PROJ_ORTHO)
		return (CreateOrthoProjMatrice());

	return ((Mat4x4){ 0 });
}

Mat4x4	Mat4x4Translation(Vec3 v)
{
	Mat4x4	m;

	m = (Mat4x4){ 
		.a = (Vec4) { 1, 0, 0, 0 },
		.b = (Vec4) { 0, 1, 0, 0 },
		.c = (Vec4) { 0, 0, 1, 0 },
		.d = (Vec4) { v.x, v.y, v.z, 1 }
	};

	return (m);
}

Mat4x4		Mat4x4Identity()
{
	Mat4x4	m;

	m = (Mat4x4){ 
		.a = (Vec4) { 1, 0, 0, 0 },
		.b = (Vec4) { 0, 1, 0, 0 },
		.c = (Vec4) { 0, 0, 1, 0 },
		.d = (Vec4) { 0, 0, 0, 1 }
	};

	return (m);
}

void		TranslateMat4x4(Mat4x4 *m, Vec3 v)
{
	m->d.x = v.x;
	m->d.y = v.y;
	m->d.z = v.z;
}

void		ScaleMat4x4(Mat4x4 *m, Vec3 v)
{
	m->a.x = v.x;
	m->b.y = v.y;
	m->c.z = v.z;
}

