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
	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
			return 0;
		Vector2 WindowSize = { (FLOAT)LOWORD(lParam), (FLOAT)HIWORD(lParam) };
		EngineCoreEvents->FireEvent("WindowUpdated", &WindowSize);
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
		if (wParam == VK_ESCAPE)
		{
			EngineCoreEvents->FireEvent("WindowDestroyed");
		}
		return 0;
		break;
	default:
		return DefWindowProcW(hwnd, msg, wParam, lParam);
	}
}

Window::Window(Vector2 size)
{
	m_size = size;
	m_hwnd = HWND();
}

#ifndef SL_ENGINE_EDITOR

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
		WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW,
		GetSystemMetrics(SM_CXSCREEN) / 2 - (int)m_size.X / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - (int)m_size.Y / 2,
		(int)m_size.X, (int)m_size.Y,
		NULL, NULL, NULL, NULL);


	if (!m_hwnd)
		return FALSE;

	hwnd = m_hwnd;

	EngineCoreEvents->AddListener([&](void) { isRunning = false; DestroyWindow(m_hwnd); }, "WindowDestroyed");
	EngineCoreEvents->AddListener<Vector2>([&](Vector2 newSize) {m_size = newSize; }, "WindowUpdated");


	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	isRunning = TRUE;

	return TRUE;
}

#else

LRESULT Window::RenderWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
		EngineCoreEvents->FireEvent("WindowRenderUpdated", &WindowSize);
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
		if (wParam == VK_ESCAPE)
		{
			EngineCoreEvents->FireEvent("WindowDestroyed");
		}
		return 0;
		break;
	default:
		return DefWindowProcW(hwnd, msg, wParam, lParam);
	}
}

bool Window::Initialize(LPCWSTR title, HWND& hwnd, HWND& renderHWND)
{
	WNDCLASSEX wcex;
	WNDCLASSEX wcexRender;

	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = NULL;
	wcex.lpszClassName = title;

	RegisterClassEx(&wcex);

	m_hwnd = CreateWindowExW(NULL,
		title, title,
		WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_BORDER | WS_CLIPCHILDREN,
		0, 0,
		GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		NULL, NULL, NULL, NULL);

	ZeroMemory(&wcexRender, sizeof(WNDCLASSEX));

	wcexRender.cbSize = sizeof(WNDCLASSEX);
	wcexRender.lpfnWndProc = WndProc;
	wcexRender.hInstance = NULL;
	wcexRender.lpszClassName = L"RenderWindow";

	RegisterClassEx(&wcexRender);

	m_hwndRender = CreateWindowExW(NULL,
		L"RenderWindow", L"Render",
		WS_OVERLAPPEDWINDOW | WS_CHILDWINDOW | WS_CLIPSIBLINGS,
		0, 0,
		GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2,
		m_hwnd, NULL, NULL, NULL);



	if (!m_hwnd || !m_hwndRender)
		return FALSE;

	m_sizeRender = Vector2(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);

	EngineCoreEvents->AddListener([&](void) { isRunning = false; DestroyWindow(m_hwnd); }, "WindowDestroyed");
	EngineCoreEvents->AddListener<Vector2>([&](Vector2 newSize) {m_size = newSize; }, "WindowUpdated");
	EngineCoreEvents->AddListener<Vector2>([&](Vector2 newSize) {m_sizeRender = newSize; }, "WindowRenderUpdated");

	ShowWindow(m_hwnd, SW_SHOW);
	ShowWindow(m_hwndRender, SW_SHOW);
	UpdateWindow(m_hwnd);

	hwnd = m_hwnd;
	renderHWND = m_hwndRender;

	isRunning = TRUE;
	return TRUE;
}

Vector2 Window::GetRenderSize()
{
	return m_sizeRender;
}
#endif

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
