#include "test.h"

typedef enum {
	OFFSET_CMP = DEFAULT_COMPONENT_COUNT,
}	ComponentLocal;

typedef enum {
	OFFSET_ETP = DEFAULT_EVENT_TYPE_COUNT,
}	EventTypeIDLocal;

typedef struct {
	Vec2	position;
	Vec2	velocity;
	i32		acceleration;
	i32		deceleration;
	i32		maxSpeed;
}	Transform;

void	*defaultTransform()
{
	Transform	t;
	Transform	*t_ptr;

	t.position = (Vec2){ 0, 0 };
	t.velocity = (Vec2){ 0, 0 };
	t.acceleration = 0;
	t.deceleration = 0;
	t.maxSpeed = 0;
	t_ptr = &t;
	return (t_ptr);
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	if (!CreateInstance("Zombie Game", 1080, 720)) {
		printf("Failed to create engine\n");
		return (1);
	}

	LaunchInstance();
	DestroyInstance();
	return (0);	
}
