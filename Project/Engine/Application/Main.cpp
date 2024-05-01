#include "pch.h"
#include "App.h"

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd)
{
	App NewWindow;

	if (!NewWindow.Initialize())
		return 1;

	NewWindow.Run();

	NewWindow.Shutdown();

	return 0;
}