#include "Window.h"

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT nCmdShow)
{
	Window window(1000, 1000, L"Program", hInstance);

		
	ShowWindow(window.GetHWND(), nCmdShow);
	
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}