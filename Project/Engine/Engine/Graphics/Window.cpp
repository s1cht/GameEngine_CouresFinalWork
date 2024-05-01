#include "pch.h"
#include "Window.h"
#include "Engine/Core/Events/CoreEvents.h"


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
		Vector2 WindowSize = { (FLOAT)rc.right, (FLOAT)rc.bottom };
		EngineCoreEvents->FireEvent("WindowUpdated", &WindowSize);
		return 0;
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
		break;
	}
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			EngineCoreEvents->FireEvent("WindowDestroyed");
		}
		return 0;
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

Window::Window(Vector2 size)
{
	m_size = size;
	m_hwnd = HWND();
}

//Window::Window(const Window& w) {}

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
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		GetSystemMetrics(SM_CXSCREEN) / 2 - (int)m_size.X / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - (int)m_size.Y / 2,
		(int)m_size.X, (int)m_size.Y,
		NULL, NULL, NULL, NULL);

	hwnd = m_hwnd;

	if (!hwnd)
		return FALSE;

	EngineCoreEvents->AddListener([&](void) { isRunning = false; DestroyWindow(m_hwnd); }, "WindowDestroyed");
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
