#include "game.h"

static void	loadTestDUI();

void	LoadGameContent()
{
	LoadNewPlayer();

	loadTestDUI();
}

static void loadTestDUI()
{
	DebugUIWindow	*windowTest = CreateDUIWindow();
	RegisterDUIWindow(windowTest);
}
