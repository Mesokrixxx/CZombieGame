# include <GE.h>

# define CIRCLE_SEGMENTS 32

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

typedef struct {
	f32		radius, outlineSize;
	Color	color, outlineColor;
}	CircleComp;

typedef enum {
	OFFSET = GE_COMP_COUNT,
	POS_COMP,
	VEL_COMP,
	SPRITE_COMP,
	_CIRCLE,
}	LocalComponent;

# define CIRCLE_COMP (SPRITE_COMP | _CIRCLE)

# define WINDOW_WIDTH 1080
# define WINDOW_HEIGHT 720

void LoadContent(GEInstance *instance);
void InitContent(GEInstance *instance);

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
static void	*_DefaultCircleCompCreator();
static void	_movementSystem(void *, u32, f32);
static void	_renderSystem(void *, u32, f32);
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
		POS_COMP | VEL_COMP, _movementSystem));

	GERegisterSystem(instance->ecs, GECreateSystem(
		POS_COMP | SPRITE_COMP, _renderSystem));

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

static void _movementSystem(void *vinstance, u32 entityID, f32 dt)
{
	GEInstance	*instance = vinstance;
	Vec2	*entityPos = GEGetComponent(instance->ecs, POS_COMP, entityID);
	VelComp	*entityVel = GEGetComponent(instance->ecs, VEL_COMP, entityID);

	if (entityVel->velocity.x != 0 && entityVel->velocity.y != 0)
	{
		f32 currentSpeed = LenghtVec2(entityVel->velocity);
		
		// Cap speed
		if (currentSpeed > entityVel->maxSpeed && entityVel->maxSpeed > -1) // if -1 infinite speed
		{
			f32 scale = (entityVel->maxSpeed / currentSpeed);

			entityVel->velocity = ScaleVec2(entityVel->velocity, scale);
			currentSpeed *= scale;
		}

		// Decelerate
		if (entityVel->deceleration > 1)
			entityVel->deceleration = 1;
		if (currentSpeed > 0 && entityVel->deceleration > 0) // if deceleration equal to 0 then no deceleration
		{
			f32 scale = 1 - entityVel->deceleration * dt;
			entityVel->velocity = ScaleVec2(entityVel->velocity, scale > 0 ? scale : 0);
		}

		// Update Pos
		*entityPos = AddVec2s(*entityPos, ScaleVec2(entityVel->velocity, dt));
	}
}

static void _renderSystem(void *vinstance, u32 entityID, f32 dt)
{
	GEInstance	*instance = vinstance;
	Vec2		*entityPos = GEGetComponent(instance->ecs, POS_COMP, entityID);
	CircleComp	*entityCircle = GEGetComponent(instance->ecs, CIRCLE_COMP, entityID);

	GLuint shaderProg = GEUseShader(instance->graphics, SHADERPROG_CIRCLE);
	GEUseVAO(instance->graphics, VO_CIRCLE);

	GLint modelLoc = glGetUniformLocation(shaderProg, "model");
	GLint colorLoc = glGetUniformLocation(shaderProg, "color");
	f32	matrice[16];

	Mat4x4	model = Mat4x4Identity();
	TranslateMat4x4(&model, Vec3FromVec2(*entityPos, 0));
	ScaleMat4x4(&model, newVec3(entityCircle->radius, entityCircle->radius, 1));
	GEMat4x4ToFloat(matrice, model);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, matrice);

	glUniform4f(colorLoc,
		entityCircle->color.r, entityCircle->color.g, entityCircle->color.b, entityCircle->color.a);

	glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_SEGMENTS);
}

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

static void	_testKeyDownListeners(void *vinstance, void *data, u32 linkedID)
{
	SDL_Scancode code = *(SDL_Scancode *)data;

	GEInstance	*instance = vinstance;
	VelComp		*vel = GEGetComponent(instance->ecs, VEL_COMP, linkedID);

	if (code == SDL_SCANCODE_W)
		vel->velocity.y += -vel->acceleration;
	else if (code == SDL_SCANCODE_S)
		vel->velocity.y += vel->acceleration;
	
	if (code == SDL_SCANCODE_A)
		vel->velocity.x += -vel->acceleration;
	else if (code == SDL_SCANCODE_D)
		vel->velocity.x += vel->acceleration;
}

void	InitContent(GEInstance *instance)
{
	u32 playerTest = GECreateEntity(instance->ecs, POS_COMP | VEL_COMP | CIRCLE_COMP);
	Vec2 *pos = GEGetComponent(instance->ecs, POS_COMP, playerTest);
	VelComp *vel = GEGetComponent(instance->ecs, VEL_COMP, playerTest);
	CircleComp *circle = GEGetComponent(instance->ecs, CIRCLE_COMP, playerTest);

	*pos = newVec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	vel->velocity = newVec2(0, 0);
	vel->acceleration = 50;
	vel->deceleration = vel->acceleration * 2;
	vel->maxSpeed = 200;

	circle->radius = 16;
	circle->color = GE_COLOR_LIGHT_GRAY;

	GEAddEventListener(instance->eventBus, GE_EVENT_TYPE_KEYDOWN, GECreateEventListener(_testKeyDownListeners, playerTest));
}
