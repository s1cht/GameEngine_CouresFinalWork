#include "Window.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			// All painting occurs here, between BeginPaint and EndPaint.

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
	
			EndPaint(hwnd, &ps);
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}

Window::Window(INT w, INT h, const WCHAR className[], HINSTANCE hInstance) : w_width(w), w_heigth(h)
{	
	WNDCLASSEX wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = HINSTANCE();
	wcex.lpszClassName = className;

	RegisterClassEx(&wcex);

	m_HWND = CreateWindowExW(0, className, L"Welcome!", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, w_width, w_heigth, NULL, NULL, hInstance, NULL);
	
	if (m_HWND == NULL)
	{
		PostQuitMessage(0);
	}
}

Window::~Window()
{
}

HWND Window::GetHWND()
{
	return m_HWND;
}
