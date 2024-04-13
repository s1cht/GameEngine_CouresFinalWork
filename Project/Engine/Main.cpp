#include "pch.h"
#include "App.h"

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShowCmd)
{
	App NewWindow;

	while (NewWindow.Runs())
	{
		NewWindow.Update();
	}

	return 0;
}