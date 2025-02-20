#include "GE.h"

extern Instance	*instance;

Bool	InitDefaultContent()
{
	if (!InitDefaultEventType()) return (false);
	if (!InitDefaultEventListnerer()) return (false);
	if (!InitDefaultComponents()) return (false);
	if (!InitDefaultShaderProgram()) return (false);
	if (!InitDefaultVOs()) return (false);

	return (true);
}

static void	*keyCreator(void);
static void	*mouseclickCreator(void);
static void	mouseClickRemover(void *data);
static void	*scrollCreator(void);

static void defaultEndInstance(void *);
static void	defaultKeyDownListener(void *data);
static void	defaultKeyUpListener(void *data);

Bool	justSwitchedDMode = false;

Bool	InitDefaultEventType()
{
	if (!RegisterEventType(KEYDOWN_EVTP, keyCreator, NULL)) return (false);
	if (!RegisterEventType(KEYUP_EVTP, keyCreator, NULL)) return (false);
	if (!RegisterEventType(MOUSEDOWN_EVTP, mouseclickCreator, mouseClickRemover)) return (false);
	if (!RegisterEventType(MOUSEUP_EVTP, mouseclickCreator, mouseClickRemover)) return (false);
	if (!RegisterEventType(SCROLL_EVTP, scrollCreator, NULL)) return (false);
	if (!RegisterEventType(QUIT_EVTP, NULL, NULL)) return (false);

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
	MouseEvent	*button;

	button = _malloc(sizeof(MouseEvent));
	button->button = SDL_BUTTON_LEFT;
	button->pos = _malloc(sizeof(Vec2));
	return (button);
}

static void	mouseClickRemover(void *data)
{
	MouseEvent	*evData = data;

	_free(evData->pos);
	_free(evData);
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

static void testMouseListener(void *data)
{
	printf("Mouse move: %d %d\n", ((MouseEvent *)data)->pos->x, ((MouseEvent *)data)->pos->y);
}

Bool	InitDefaultEventListnerer()
{
	if (!NewEventListener(QUIT_EVTP, defaultEndInstance)) return (false);
	if (!NewEventListener(KEYDOWN_EVTP, defaultKeyDownListener)) return (false);
	if (!NewEventListener(KEYUP_EVTP, defaultKeyUpListener)) return (false);
	if (!NewEventListener(MOUSEDOWN_EVTP, testMouseListener)) return (false);
	if (!NewEventListener(MOUSEUP_EVTP, testMouseListener)) return (false);

	return (true);
}

static void	defaultEndInstance(void *data)
{
	(void)data;
	instance->running = false;
}

static void	defaultKeyDownListener(void *data)
{
	SDL_Scancode	s = *(SDL_Scancode *)data;

	if (s == SDL_SCANCODE_G && !justSwitchedDMode)
	{
		instance->debugMode = !instance->debugMode;
		justSwitchedDMode = true;
	}
}

static void	defaultKeyUpListener(void *data)
{
	SDL_Scancode	s = *(SDL_Scancode *)data;

	if (s == SDL_SCANCODE_G)
		justSwitchedDMode = false;
}

Bool	InitDefaultComponents()
{
	if (!RegisterComponent(FLAGS_CMP, sizeof(u32), NULL, NULL)) return (false);

	return (true);
}

Bool	InitDefaultShaderProgram()
{
	GLuint defaultCircle = CreateShaderProgram(
		"libGE/src/res/shaders/defaultCircle.vert",
		"libGE/src/res/shaders/defaultCircle.frag");

	if (!AddToSparseSet(instance->shaderPrograms, &defaultCircle, SHADERPROG_CIRCLE_DEFAULT)) 
		return (false);

	GLuint debugUI = CreateShaderProgram(
		"libGE/src/res/shaders/debugUI.vert",
		"libGE/src/res/shaders/debugUI.frag");
	
	if (!AddToSparseSet(instance->shaderPrograms, &debugUI, SHADERPROG_DEBUGUI))
		return (false);

	return (true);
}

Bool	InitDefaultVOs()
{
	GLuint	circleVAO, circleVBO;

	CreateCirleVAO(&circleVAO, &circleVBO, DEFAULT_CIRCLE_ROUNDNESS);
	if (!RegisterVertexObject((VertexObject){ circleVAO, circleVBO }, CIRCLE_VO)) return (false);

	GLuint	debugUIVAO, debugUIVBO;

	CreateRectVAO(&debugUIVAO, &debugUIVBO);
	if (!RegisterVertexObject((VertexObject){ debugUIVAO, debugUIVBO }, RECT_VO)) return (false);

	return (true);
}
