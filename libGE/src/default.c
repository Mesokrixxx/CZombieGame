#include "GE.h"

extern Instance	*instance;

Bool	InitDefaultContent()
{
	if (!InitDefaultEventType()) return (false);
	if (!InitDefaultEventListnerer()) return (false);
	if (!InitDefaultComponents()) return (false);

	return (true);
}

static void	*keyCreator(void);
static void	*mouseclickCreator(void);
static void	*scrollCreator(void);

static void defaultEndInstance(void *);

Bool	InitDefaultEventType()
{
	if (!RegisterEventType("keydown_event", keyCreator, NULL)) return (false);
	if (!RegisterEventType("keyup_event", keyCreator, NULL)) return (false);
	if (!RegisterEventType("mousedown_event", mouseclickCreator, NULL)) return (false);
	if (!RegisterEventType("mouseup_event", mouseclickCreator, NULL)) return (false);
	if (!RegisterEventType("scroll_event", scrollCreator, NULL)) return (false);
	if (!RegisterEventType("quit_event", NULL, NULL)) return (false);

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
	iVec2	v;
	iVec2	*v_ptr;

	v.x = 0;
	v.y = 0;
	v_ptr = &v;
	return (v_ptr);
}

Bool	InitDefaultEventListnerer()
{
	if (!NewEventListener("quit_event", defaultEndInstance)) return (false);

	return (true);
}

static void	defaultEndInstance(void *data)
{
	(void)data;
	instance->running = false;
}

Bool	InitDefaultComponents()
{
	if (!RegisterComponent(FLAGS_CMP, sizeof(u32), NULL, NULL)) return (false);

	return (true);
}
