#include "Window.h"


LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

}

bool Window::Create(LPCWSTR title, HINSTANCE hInstance, INT nShowCmd)
{
    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = winClass;

    RegisterClassEx(&wcex);

    m_hwnd = CreateWindowExW(NULL,
        winClass, title,
        WS_OVERLAPPEDWINDOW | WS_MAXIMIZE, 
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, heigth,
        NULL, NULL, hInstance, NULL);

    if (!m_hwnd)
        return FALSE;

    ShowWindow(m_hwnd, nShowCmd);
    return TRUE;
}

Window::~Window()
{
}
