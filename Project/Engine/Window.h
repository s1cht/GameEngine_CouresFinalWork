#pragma once
#include <Windows.h>

const LPCWSTR winClass = L"Engine";

class Window
{
private:
	HWND m_hwnd;
	int width, heigth;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Window(INT w, INT h) : width(w), heigth(h), m_hwnd(HWND()) {};
	bool Create(LPCWSTR title, HINSTANCE hInstance, INT nShowCmd);
	~Window();
};

