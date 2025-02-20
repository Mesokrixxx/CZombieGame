#ifndef GAMEENGINE_DEBUGUI_H
# define GAMEENGINE_DEBUGUI_H

# include "vars.h"

# define MAX_DUI_WINDOWS		12
# define MAX_COMP_DUI_WINDOWS	56

# define MIN_DUI_WINDOW_WIDTH	112
# define MIN_DUI_WINDOW_HEIGT	24
# define MAX_DUI_WINDOW_WIDTH	480
# define MAX_DUI_WINDOW_HEIGHT	720

typedef struct DebugUIComp {
	union {
		i32		*idata;
		f32		*fdata;
		char	*strdata;
		Vec2	*vec2data;
		Color	*coldata;
		Bool	*booldata;
	};
	void	(*action)(void *data);
}	DebugUIComp;

void			DestroyDUIComp(DebugUIComp *comp);

typedef struct DebugUIWindow {
	u32			id;
	
	DebugUIComp	**comps;
	u32			compCount;
	Vec2		*pos;

	i32			currWidth;
	i32			currHeight;
	Bool		moveWindow;
	Bool		resizeWindow;
}	DebugUIWindow;

void			DestroyDUIWindow(DebugUIWindow *window);

typedef struct DebugUI {
	DebugUIWindow	**windows;
	u32				windowCount;
}	DebugUI;

DebugUI			*CreateUIDebugger();
void			DestroyUIDebugger(DebugUI *debugUI);

void			RenderDebugUI(DebugUI *debugUI);
void			RenderDUIWindow(DebugUIWindow *window);
DebugUIWindow	*GetDUIWindow(iVec2 pos);
void			HandleDUIMouseDown(MouseEvent *mouse);
void			HandleDUIMouseUp(MouseEvent *mouse);

#endif