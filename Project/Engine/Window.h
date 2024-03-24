#pragma once

#include <windows.h>

class Window
{
private:
	INT w_width, w_heigth;
	HWND m_HWND;
public:
	Window(INT, INT, const WCHAR[], HINSTANCE);
	~Window();
	
	HWND GetHWND();
};

