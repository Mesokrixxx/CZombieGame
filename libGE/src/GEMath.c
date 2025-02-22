#include "GEPrivate.h"

static Mat4x4	_createOrthoProjMatrice(iVec2 size);

Vec2	newVec2(f32 x, f32 y) { return ((Vec2){ x, y }); }
iVec2	newIVec2(i32 x, i32 y) { return ((iVec2){ x, y }); }
Vec3	newVec3(f32 x, f32 y, f32 z) { return ((Vec3){ x, y, z }); }
iVec3	newIVec3(i32 x, i32 y, i32 z) { return ((iVec3){ x, y, z }); }

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

	f32	right = 0.0f, left = size.x;
	f32	top = 0.0f, bottom = size.y;
	f32 nearP = -1.0f, farP = 1.0f;

	m = (Mat4x4){ 
		.a = (Vec4) { 2 / (right - left), 0, 0, 0 },
		.b = (Vec4) { 0, 2 / (top - bottom), 0, 0 },
		.c = (Vec4) { 0, 0, 2 / (farP - nearP), 0 },
		.d = (Vec4) { -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(farP + nearP) / (farP - nearP), 1 }
	};

	return (m);
}
