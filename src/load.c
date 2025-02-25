#include "zombieGame.h"

static void	*_DefaultVec2Creator();
static void	*_DefaultVelCompCreator();
static void	*_DefaultCircleCompCreator();
static bool	_createCircleVAO(GLuint *, GLuint *, u32);

void	LoadContent(GEInstance *instance)
{
	GEVertexObject circleVO; GLuint circleVAO, circleVBO;

	GERegisterComponent(instance->ecs, GECreateComponent(
		POS_COMP, sizeof(Vec2), _DefaultVec2Creator, NULL));
	
	GERegisterComponent(instance->ecs, GECreateComponent(
		VEL_COMP, sizeof(VelComp), _DefaultVelCompCreator, NULL));

	GERegisterComponent(instance->ecs, GECreateComponent(
		CIRCLE_COMP, sizeof(CircleComp), _DefaultCircleCompCreator, NULL));

	GERegisterSystem(instance->ecs, GECreateSystem(
		POS_COMP | VEL_COMP, MovementSystem));

	GERegisterSystem(instance->ecs, GECreateSystem(
		POS_COMP | SPRITE_COMP, RenderSystem));

	GERegisterShaderProgram(instance->graphics,
		"src/res/shaders/circle.vert", "src/res/shaders/circle.frag", SHADERPROG_CIRCLE);

	_createCircleVAO(&circleVAO, &circleVBO, CIRCLE_SEGMENTS);
	circleVO = GECreateVertexObject(circleVAO, circleVBO);
	GERegisterVO(instance->graphics, circleVO, VO_CIRCLE);

	InitContent(instance);
}

static void	*_DefaultVec2Creator() { return (&(Vec2){ 0 }); }
static void	*_DefaultVelCompCreator() { return (&(VelComp){ 0 }); }
static void	*_DefaultCircleCompCreator() { return (&(CircleComp){ 0 }); }

static bool	_createCircleVAO(GLuint *vao, GLuint *vbo, u32 segments)
{
	f32		*vertices;
	
	vertices = malloc(sizeof(f32) * segments * 2);
	if (!vertices)
		return (false);

	f32	angleStep = (GE_PI * 2.0f) / segments;
	for (i32 i = 0; i < segments; i++)
	{
		float	theta = angleStep * i;

		vertices[i * 2] = SDL_cosf(theta);
		vertices[i * 2 + 1] = SDL_sinf(theta);
	}

	glGenVertexArrays(1, vao);
	glGenBuffers(1, vbo);

	glBindVertexArray(*vao);

	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * segments * 2, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	free(vertices);
	return (true);
}
