# include <GE.h>

typedef enum {
	SHADERPROG_CIRCLE,
}	ShaderProgramIDs;

typedef enum {
	VO_CIRCLE,
}	VOIDs;

typedef struct {
	Vec2	velocity;
	f32		acceleration;
	f32		deceleration;
	f32		maxSpeed;
}	VelComp;

typedef enum {
	OFFSET = GE_COMP_COUNT,
	POS_COMP,
	VEL_COMP,
}	LocalComponent;

# define WINDOW_WIDTH 1080
# define WINDOW_HEIGHT 720

void LoadContent(GEInstance *instance);

int main(int argc, char **argv)
{
	GEInstance	*mainInstance;
	
	mainInstance = malloc(sizeof(GEInstance));
	if (!mainInstance)
	{
		printf("Failed to allocate for mainInstance\n");
		return (1);
	}

	GECreateInstance(mainInstance, "Zombie Game", newIVec2(WINDOW_WIDTH, WINDOW_HEIGHT), GE_PROJECTION_ORTHO);

	LoadContent(mainInstance);

	u32 entityTest = GECreateEntity(mainInstance->ecs, POS_COMP | VEL_COMP);

	GELaunchInstance(mainInstance);
	GEDestroyInstance(mainInstance);

	free(mainInstance);

	return (0);	
}

static void	*_DefaultVec2Creator();
static void	*_DefaultVelCompCreator();
static bool	_createCircleVAO(GLuint *, GLuint *, u32);

void	LoadContent(GEInstance *instance)
{
	GEVertexObject circleVO; GLuint circleVAO, circleVBO;

	GERegisterComponent(instance->ecs, GECreateComponent(
		POS_COMP, sizeof(Vec2), _DefaultVec2Creator, NULL));
	
	GERegisterComponent(instance->ecs, GECreateComponent(
		VEL_COMP, sizeof(VelComp), _DefaultVelCompCreator, NULL));

	GERegisterShaderProgram(instance->graphics,
		"src/res/shaders/circle.vert", "src/res/shaders/circle.frag", SHADERPROG_CIRCLE);
	
	if (!_createCircleVAO(&circleVAO, &circleVBO, 32))
		return ((void)printf("Failed to create cirle VAO\n"));
	circleVO = GECreateVertexObject(circleVAO, circleVBO);
	GERegisterVO(instance->graphics, &circleVO, VO_CIRCLE);
}

static void	*_DefaultVec2Creator() { return (&(Vec2){ 0 }); }
static void	*_DefaultVelCompCreator() { return (&(VelComp){ 0 }); }

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
