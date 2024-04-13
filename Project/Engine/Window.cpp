#include "pch.h"
#include "Window.h"
#include "CoreEvents.h"

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
	{
        EngineCoreEvents.FireEvent("WindowDestroyed");
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

bool Window::Create(LPCWSTR title)
{
    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = NULL;
    wcex.lpszClassName = winClass;

    RegisterClassEx(&wcex);

    m_hwnd = CreateWindowExW(NULL,
        winClass, title,
        WS_OVERLAPPEDWINDOW | WS_MAXIMIZE, 
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL, NULL, NULL, NULL);

    if (!m_hwnd)
        return FALSE;

    EngineCoreEvents.RegisterCoreEvent("WindowDestroyed", &destroy);
    EngineCoreEvents.AddListener([&](void) { isRunning = false; return; }, "WindowDestroyed");

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    isRunning = TRUE;

    return TRUE;
}

bool Window::Runs()
{
    return isRunning;
}

Window::~Window()
{
}
