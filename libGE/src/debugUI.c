#include "GE.h"

extern Instance	*instance;

DebugUI	*CreateUIDebugger()
{
	DebugUI	*ui;

	ui = _malloc(sizeof(DebugUI));
	if (!ui)
		return ((void)LOG("Failed to allocate for UI Debugger\n"), NULL);

	ui->windows = _malloc(sizeof(DebugUIWindow *) * MAX_DUI_WINDOWS);
	if (!ui->windows)
		return ((void)LOG("Failed to allocate for uidebugger windows list\n"), _free(ui), NULL);
	memset(ui->windows, 0, sizeof(DebugUIWindow *) * MAX_DUI_WINDOWS);

	ui->windowCount = 0;

	return (ui);
}

void	DestroyUIDebugger(DebugUI *debugUI)
{
	for (u32 i = 0; i < debugUI->windowCount; i++)
		DestroyDUIWindow(debugUI->windows[i]);
	_free(debugUI->windows);
	_free(debugUI);
}

void	RenderDebugUI(DebugUI *debugUI)
{
	for (u32 i = 0; i < debugUI->windowCount; i++)
		RenderDUIWindow(debugUI->windows[i]);
}

void	RegisterDUIWindow(DebugUIWindow *window)
{
	DebugUI	*DUI = instance->debugUI;
	
	if (DUI->windowCount >= MAX_DUI_WINDOWS)
		return ;
	DUI->windows[DUI->windowCount] = window;
	DUI->windowCount++;
}

DebugUIWindow	*CreateDUIWindow()
{
	DebugUIWindow	*window;

	window = _malloc(sizeof(DebugUIWindow));
	if (!window)
		return ((void)LOG("Failed to allocate for new window\n"), NULL);
	
	window->comps = _malloc(sizeof(DebugUIComp *) * MAX_COMP_DUI_WINDOWS);
	if (!window->comps)
		return ((void)LOG("Failed to allocate for comps in the new window\n"), _free(window->comps), NULL);
	memset(window->comps, 0, sizeof(DebugUIComp *) * MAX_COMP_DUI_WINDOWS);

	window->compCount = 0;
	window->pos = (Vec2){ instance->windowParam.w / 2, instance->windowParam.h / 2 };
	window->currWidth = MIN_DUI_WINDOW_WIDTH;
	window->currHeight = MIN_DUI_WINDOW_HEIGT;

	return (window);
}

void	DestroyDUIWindow(DebugUIWindow *window)
{
	for (u32 i = 0; i < window->compCount; i++)
		DestroyDUIComp(window->comps[i]);
	_free(window->comps);
	_free(window);
}

void	RenderDUIWindow(DebugUIWindow *window)
{
	GLuint	shaderProg = GetShaderProgram(SHADERPROG_DEBUGUI);

	UseShader(shaderProg);

	glBindVertexArray(GetVAO(RECT_VO));

	Mat4x4	model = Mat4x4Identity();
	TranslateMat4x4(&model, Vec3FromVec2(window->pos, 0));
	ScaleMat4x4(&model, (Vec3){ window->currWidth, window->currHeight, 1 });

	GLint	modelLoc = glGetUniformLocation(shaderProg, "model");
	float	matrix[16];

	Mat4x4ToFloat(model, matrix);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, matrix);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
}

void	DestroyDUIComp(DebugUIComp *comp)
{
	_free(comp);
}
