# include <GE.h>

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
	
	// SetInstanceBGCol(GE_COLOR_LIGHTGRAY);

	u32 entityTest = GECreateEntity(mainInstance->ecs, POS_COMP | VEL_COMP);

	GELaunchInstance(mainInstance);
	GEDestroyInstance(mainInstance);

	free(mainInstance);

	return (0);	
}

static void	*_DefaultVec2Creator();
static void	*_DefaultVelCompCreator();

void	LoadContent(GEInstance *instance)
{
	GERegisterComponent(instance->ecs, GECreateComponent(
		POS_COMP, sizeof(Vec2), _DefaultVec2Creator, NULL));
	
	GERegisterComponent(instance->ecs, GECreateComponent(
		VEL_COMP, sizeof(VelComp), _DefaultVelCompCreator, NULL));
}

static void	*_DefaultVec2Creator() { return (&(Vec2){ 0 }); }
static void	*_DefaultVelCompCreator() { return (&(VelComp){ 0 }); }
