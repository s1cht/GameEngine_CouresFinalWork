#include "pch.h"
#include "Window.h"
#include "Engine/Core/Events/CoreEvents.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_ACTIVATEAPP:
	{
		DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
	}
	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
			return 0;
		ENG_PTR<Vector2> WindowSize = TO_ENG_PTR<Vector2>((FLOAT)LOWORD(lParam), (FLOAT)HIWORD(lParam)); //{ (FLOAT)LOWORD(lParam), (FLOAT)HIWORD(lParam) };
		EngineCoreEvents->FireEvent("WindowUpdated", WindowSize);
		return 0;
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		RECT rc;
		HDC hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		return 0;
		break;
	}
	case WM_DESTROY:
	{
		EngineCoreEvents->FireEvent("WindowDestroyed");
		PostQuitMessage(0);
		return 0;
		break;
	}
	case WM_KEYUP:
#ifndef SL_ENGINE_EDITOR
		if (wParam == VK_ESCAPE)
		{
			EngineCoreEvents->FireEvent("WindowDestroyed");
		}
#endif
		DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
	case WM_KEYDOWN:
	{
		DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
	}
	default:
		return DefWindowProcW(hwnd, msg, wParam, lParam);
	}
}

Window::Window(Vector2 size)
{
	m_size = size;
	m_hwnd = HWND();
}

bool Window::Initialize(LPCWSTR title, HWND& hwnd)
{
	WNDCLASSEX wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = NULL;
	wcex.lpszClassName = title;

	RegisterClassEx(&wcex);

	m_hwnd = CreateWindowExW(NULL,
		title, title,
		WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW | WS_MAXIMIZE, 
		0, 0,
		(int)m_size.X, (int)m_size.Y,
		NULL, NULL, NULL, NULL);


	if (!m_hwnd)
		return FALSE;

	hwnd = m_hwnd;

	EngineCoreEvents->AddListener([&](void) { isRunning = false; DestroyWindow(m_hwnd); }, "WindowDestroyed");
	EngineCoreEvents->AddListener<Vector2>([&](EVENT_RET_TYPE<Vector2> newSize) {m_size.X = newSize->X; m_size.Y = newSize->Y; }, "WindowUpdated");

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
