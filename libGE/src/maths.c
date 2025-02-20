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

Vec4	AddVec4s(Vec4 v1, Vec4 v2)
{
	return ((Vec4){ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w });
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

	f32	left = 0.0f, right = instance->windowParam.w;
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

Mat4x4 Mat4x4MulMat4x4(Mat4x4 a, Mat4x4 b)
{
	Mat4x4	c;

	c.a.x = a.a.x * b.a.x + a.b.x * b.a.y + a.c.x * b.a.z + a.d.x * b.a.w;
	c.a.y = a.a.y * b.a.x + a.b.y * b.a.y + a.c.y * b.a.z + a.d.y * b.a.w;
	c.a.z = a.a.z * b.a.x + a.b.z * b.a.y + a.c.z * b.a.z + a.d.z * b.a.w;
	c.a.w = a.a.w * b.a.x + a.b.w * b.a.y + a.c.w * b.a.z + a.d.w * b.a.w;

	c.b.x = a.a.x * b.b.x + a.b.x * b.b.y + a.c.x * b.b.z + a.d.x * b.b.w;
	c.b.y = a.a.y * b.b.x + a.b.y * b.b.y + a.c.y * b.b.z + a.d.y * b.b.w;
	c.b.z = a.a.z * b.b.x + a.b.z * b.b.y + a.c.z * b.b.z + a.d.z * b.b.w;
	c.b.w = a.a.w * b.b.x + a.b.w * b.b.y + a.c.w * b.b.z + a.d.w * b.b.w;

	c.c.x = a.a.x * b.c.x + a.b.x * b.c.y + a.c.x * b.c.z + a.d.x * b.c.w;
	c.c.y = a.a.y * b.c.x + a.b.y * b.c.y + a.c.y * b.c.z + a.d.y * b.c.w;
	c.c.z = a.a.z * b.c.x + a.b.z * b.c.y + a.c.z * b.c.z + a.d.z * b.c.w;
	c.c.w = a.a.w * b.c.x + a.b.w * b.c.y + a.c.w * b.c.z + a.d.w * b.c.w;

	c.d.x = a.a.x * b.d.x + a.b.x * b.d.y + a.c.x * b.d.z + a.d.x * b.d.w;
	c.d.y = a.a.y * b.d.x + a.b.y * b.d.y + a.c.y * b.d.z + a.d.y * b.d.w;
	c.d.z = a.a.z * b.d.x + a.b.z * b.d.y + a.c.z * b.d.z + a.d.z * b.d.w;
	c.d.w = a.a.w * b.d.x + a.b.w * b.d.y + a.c.w * b.d.z + a.d.w * b.d.w;

	return (c);
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
	m->d.x += v.x;
	m->d.y += v.y;
	m->d.z += v.z;
}

void		ScaleMat4x4(Mat4x4 *m, Vec3 v)
{
	m->a.x *= v.x;
	m->b.y *= v.y;
	m->c.z *= v.z;
}

void	RotateMat4x4(Mat4x4 *m, f32 angle)
{
	f32	c = SDL_cosf(angle);
	f32	s = SDL_sinf(angle);

	Mat4x4	rotation = {
		.a = (Vec4) { c, -s, 0, 0 },
		.b = (Vec4) { s, c, 0, 0 },
		.c = (Vec4) { 0, 0, 1, 0 },
		.d = (Vec4) { 0, 0, 0, 1 }
	};

	*m = Mat4x4MulMat4x4(*m, rotation);
}

f32	DegToRad(f32 deg)
{
	return (deg * M_PI / 180.0f);
}
