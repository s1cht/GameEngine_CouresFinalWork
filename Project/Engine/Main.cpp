#include "pch.h"
#include "App.h"

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd)
{
	App NewWindow;

	if (!NewWindow.Initialize())
		return 1;

	while (NewWindow.Runs())
		NewWindow.Update();

	NewWindow.Shutdown();

	return 0;
}