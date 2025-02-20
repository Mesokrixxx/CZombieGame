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
	
	DUI->windows[DUI->windowCount] = window;
	DUI->windowCount++;
}

DebugUIWindow	*CreateDUIWindow()
{
	DebugUIWindow	*window;
	DebugUI			*DUI = instance->debugUI;
	
	if (DUI->windowCount >= MAX_DUI_WINDOWS)
		return (NULL);

	window = _malloc(sizeof(DebugUIWindow));
	if (!window)
		return ((void)LOG("Failed to allocate for new window\n"), NULL);
	
	window->comps = _malloc(sizeof(DebugUIComp *) * MAX_COMP_DUI_WINDOWS);
	if (!window->comps)
		return ((void)LOG("Failed to allocate for comps in the new window\n"), _free(window->comps), NULL);
	memset(window->comps, 0, sizeof(DebugUIComp *) * MAX_COMP_DUI_WINDOWS);

	window->id = DUI->windowCount;
	window->compCount = 0;
	window->pos = _malloc(sizeof(iVec2));
	*window->pos = (Vec2){ ((instance->windowParam.w / 2) - (MIN_DUI_WINDOW_WIDTH / 2)), ((instance->windowParam.h / 2) - (MIN_DUI_WINDOW_HEIGT / 2)) };
	window->currWidth = MIN_DUI_WINDOW_WIDTH;
	window->currHeight = MIN_DUI_WINDOW_HEIGT;
	window->moveWindow = false;
	return (window);
}

void	DestroyDUIWindow(DebugUIWindow *window)
{
	for (u32 i = 0; i < window->compCount; i++)
		DestroyDUIComp(window->comps[i]);
	_free(window->pos);
	_free(window->comps);
	_free(window);
}

void	RenderDUIWindow(DebugUIWindow *window)
{
	iVec2	mousePos = GetMousePos();

	// Zone principale (gris foncé)
	DrawRect(*window->pos, (Vec2){ window->currWidth, window->currHeight }, (Color){ 0.25f, 0.25f, 0.25f, 0.85f });

	// Handler (bleu clair)
	Vec2	handlerPos = { window->pos->x + window->currWidth - 10, window->pos->y + window->currHeight - 10 };
	Vec2	handlerSize = { 10, 10 };
	
	Color	handlerColor = { 0.75f, 0.75f, 1.0f, 0.95f };
	if (mousePos.x >= handlerPos.x && mousePos.x < handlerPos.x + handlerSize.x
		&& mousePos.y >= handlerPos.y && mousePos.y < handlerPos.y + handlerSize.y)
		handlerColor = (Color){ 0.80f, 0.80f, 1.0f, 0.95f };

	DrawRect(handlerPos, handlerSize, handlerColor);
	
	// Header (bleu)
	Vec2	headerPos = *window->pos;
	Vec2	headerSize = { window->currWidth, MIN_DUI_WINDOW_HEIGT - 4 };

	Color	headerColor =  { 0.25f, 0.25f, 0.85f, 0.95f };
	if (mousePos.x >= headerPos.x && mousePos.x < headerPos.x + headerSize.x
		&& mousePos.y >= headerPos.y && mousePos.y < headerPos.y + headerSize.y)
		headerColor =  (Color){ 0.30f, 0.30f, 0.90f, 0.95f };

	DrawRect(headerPos, headerSize, headerColor);
}

void	DestroyDUIComp(DebugUIComp *comp)
{
	_free(comp);
}

DebugUIWindow	*GetDUIWindow(iVec2 pos)
{
	for (i32 i = instance->debugUI->windowCount - 1; i >= 0; i--)
	{
		DebugUIWindow	*window = instance->debugUI->windows[i];

		// If clicked inside a window
		if (pos.x >= window->pos->x && pos.x < window->pos->x + window->currWidth
			&& pos.y >= window->pos->y && pos.y < window->pos->y + window->currHeight)
			return (window);
	}
	return (NULL);
}

static DebugUIWindow	*clickedWindow = NULL;
static Bool				justCreateWindow = false;

void	HandleDUIMouseDown(MouseEvent *mouse)
{
	static iVec2			relativePosToCursor;

	if (mouse->button == SDL_BUTTON_LEFT)
	{
		if (!clickedWindow)
		{
			clickedWindow = GetDUIWindow(*mouse->pos);
			if (clickedWindow)
			{
				// Put clicked window on top
				if (clickedWindow->id != instance->debugUI->windowCount - 1)
				{
					DebugUIWindow *temp = clickedWindow;
					u32 oldId = clickedWindow->id;

					for (u32 i = oldId; i < instance->debugUI->windowCount - 1; i++)
					{
						instance->debugUI->windows[i] = instance->debugUI->windows[i + 1];
						instance->debugUI->windows[i]->id = i;
					}

					instance->debugUI->windows[instance->debugUI->windowCount - 1] = temp;
					temp->id = instance->debugUI->windowCount - 1;
				}

				// If clicking the bottom right corner then resize
				if (mouse->pos->x >= clickedWindow->pos->x + clickedWindow->currWidth - 10
					&& mouse->pos->y >= clickedWindow->pos->y + clickedWindow->currHeight - 10)
				{
					clickedWindow->resizeWindow = true;
				}

				// If clicking the top part then moving
				else if ((mouse->pos->x >= clickedWindow->pos->x && mouse->pos->x < clickedWindow->pos->x + clickedWindow->currWidth
					&& mouse->pos->y >= clickedWindow->pos->y && mouse->pos->y < clickedWindow->pos->y + MIN_DUI_WINDOW_HEIGT))
				{
					relativePosToCursor = (iVec2){ mouse->pos->x - clickedWindow->pos->x, mouse->pos->y - clickedWindow->pos->y};
					clickedWindow->moveWindow = true;
				}
			}
		}
		else if (clickedWindow->resizeWindow)
			{
			i32	difx = mouse->pos->x - clickedWindow->pos->x;
			i32	dify = mouse->pos->y - clickedWindow->pos->y;
			
			clickedWindow->currWidth = difx > MIN_DUI_WINDOW_WIDTH ? difx : MIN_DUI_WINDOW_WIDTH;
			clickedWindow->currHeight = dify > MIN_DUI_WINDOW_HEIGT ? dify : MIN_DUI_WINDOW_HEIGT;
			}
		else if (clickedWindow->moveWindow)
			{
			clickedWindow->pos->x = mouse->pos->x - relativePosToCursor.x;
			clickedWindow->pos->y = mouse->pos->y - relativePosToCursor.y;
		}
	}
	
	if (mouse->button == SDL_BUTTON_RIGHT && !justCreateWindow)
	{
		DebugUIWindow *newWindow = CreateDUIWindow();
		if (newWindow)
		{
			newWindow->pos->x = mouse->pos->x;
			newWindow->pos->y = mouse->pos->y;
			RegisterDUIWindow(newWindow);
		}
		justCreateWindow = true;
	}

}

void	HandleDUIMouseUp(MouseEvent *mouse)
{
	(void)mouse;
	if (clickedWindow)
	{
		clickedWindow->moveWindow = false;
		clickedWindow->resizeWindow = false;
		clickedWindow = NULL;
	}
	justCreateWindow = false;
}
