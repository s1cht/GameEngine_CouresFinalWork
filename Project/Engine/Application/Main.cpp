#include "pch.h"
#include "App.h"

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd)
{
	App Game;

	if (!Game.Initialize())
		return 1;

	Game.Run();

	Game.Shutdown();

	return 0;
}