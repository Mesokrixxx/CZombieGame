#include "test.h"

int main()
{
	if (!CreateInstance("Zombie Game", 1080, 720)) {
		printf("Failed to create engine\n");
		return (1);
	}

	LaunchInstance();
	DestroyInstance();
	return (0);	
}
