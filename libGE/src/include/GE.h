#ifndef GAMEENGINE_H
# define GAMEENGINE_H

# include "vars.h"
# include "sparseset.h"
# include "events.h"
# include "ecs.h"
# include "default.h"
# include "dtime.h"
# include "graphics.h"
# include "debugUI.h"

typedef struct Instance {
	SDL_Window		*window;
	SDL_GLContext	glContext;
	struct {
		i32	w;
		i32	h;
	}	windowParam;

	EventBus		*eventBus;
	
	ECS				*entities;
	
	SparseSet		*shaderPrograms;
	f32				*projectionMatrice;
	Color			bgCol;
	SparseSet		*VOs;
	
	Bool			debugMode;
	DebugUI			*debugUI;

	Bool			running;
}	Instance;

// Instance
Instance		*CreateInstance(const char *title, u32 width, u32 height, ProjType projType);
void			LaunchInstance();
void			SetInstanceBGCol(Color c);
void			DestroyInstance();
iVec2			GetMousePos();

// Events
Bool			RegisterEventType(u32 evtp, void *(*defaultCreator)(void), void (*defaultRemover)(void *));
Event			*NewEvent(u32 evtp);
void			DestroyEvent(Event *e);
Bool			NewEventListener(u32 evtp, void (*callback)(void *data));
void			PublishEvent(Event *e);

// Time
f32				GetDeltaTime();

// Entities
Bool			RegisterComponent(u32 compID, size_t compSize, void *(*defaultCreator)(void), void (*defaultRemover)(void *));
Bool			RegisterSystem(u32 requiredFlags, void (*update)(u32 entityID, f32 dt));
u32				CreateEntity(u32 flags);
void			RemoveEntity(u32 entityID);

// Graphics
void			DrawCircle(Vec2 pos, CircleSprite *cicle);
void			DrawRect(Vec2 pos, Vec2 size, Color c);
Bool			RegisterVertexObject(VertexObject vo, u32 voID);
GLuint			GetVAO(u32 voID);
GLuint			GetVBO(u32 voID);
GLuint			CreateShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath);
GLuint			GetShaderProgram(u32 shaderProgID);
void			UseShader(GLuint shaderProg);
void			BindVAO(u32 voID);

// DebugUI
DebugUIWindow	*CreateDUIWindow();
void			RegisterDUIWindow(DebugUIWindow *window);

// Utils
char			*GetFileContent(const char *filePath);

// Maths
f32				LenghtVec2Squared(Vec2 v);
f32				LenghtVec2(Vec2 v);
Vec2			ScaleVec2(Vec2 v, f32 scale);
Vec2			AddVec2s(Vec2 v1, Vec2 v2);
Vec3			Vec3FromVec2(Vec2 v, f32 z);
Vec4			Vec4FromVec2(Vec2 v, f32 z, f32 w);
Vec4			Vec4FromVec3(Vec3 v, f32 w);
void			Mat4x4ToFloat(Mat4x4 m, f32 *f);
Mat4x4			Mat4x4Identity();
Mat4x4			Mat4x4Translation(Vec3 v);
void			TranslateMat4x4(Mat4x4 *m, Vec3 v);
void			ScaleMat4x4(Mat4x4 *m, Vec3 v);

#endif