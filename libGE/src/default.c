#include "GE.h"

static void	*keyCreator(void);

static void	*mouseclickCreator(void);

static void	*scrollCreator(void);

Bool	InitDefaultContent()
{
	if (!RegisterEventType("keydown_event", keyCreator, NULL))
		return (false);
	
	if (!RegisterEventType("keyup_event", keyCreator, NULL))
		return (false);

	if (!RegisterEventType("mousedown_event", mouseclickCreator, NULL))
		return (false);

	if (!RegisterEventType("mouseup_event", mouseclickCreator, NULL))
		return (false);

	if (!RegisterEventType("scroll_event", scrollCreator, NULL))
		return (false);

	if (!RegisterEventType("quit_event", NULL, NULL))
		return (false);

	return (true);
}

static void	*keyCreator(void)
{
	SDL_Scancode	scancode;
	SDL_Scancode	*scancode_ptr;

	scancode = SDL_SCANCODE_UNKNOWN;
	scancode_ptr = &scancode;
	return (scancode_ptr);
}

static void	*mouseclickCreator(void)
{
	i8	button;
	i8	*button_ptr;

	button = SDL_BUTTON_LEFT;
	button_ptr = &button;
	return (button_ptr);
}

static void	*scrollCreator(void)
{
	Vec2	v;
	Vec2	*v_ptr;

	v.x = 0;
	v.y = 0;
	v_ptr = &v;
	return (v_ptr);
}

