#include "pch.h"
#include "Window.h"
#include "Engine/CoreEvents.h"


LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{

		PAINTSTRUCT ps;
		RECT rc;
		HDC hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		Vector2 WindowSize = { rc.right, rc.bottom };
		EngineCoreEvents->FireEvent("WindowUpdated", &WindowSize);
		break;
	}
	case WM_DESTROY:
	{
		EngineCoreEvents->FireEvent("WindowDestroyed");
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

Window::Window()
{
	m_size = { 1000.f, 1000.f };
}

Window::Window(const Window&)
{
}

bool Window::Initialize(LPCWSTR title, HWND& hwnd)
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
		m_size.X, m_size.Y,
		NULL, NULL, NULL, NULL);

	hwnd = m_hwnd;

	if (!hwnd)
		return FALSE;

	EngineCoreEvents->AddListener([&](void) { isRunning = false; MessageBox(NULL, L"g", L"LOL", NULL); }, "WindowDestroyed");
	EngineCoreEvents->AddListener<Vector2>([&](Vector2 newSize) {m_size = newSize; }, "WindowUpdated");


	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	isRunning = TRUE;

	return TRUE;
}


Vector2 Window::GetSize()
{
	return m_size;
}

bool Window::Runs()
{
	return isRunning;
}

Window::~Window()
{
}
