#pragma once
#include "pch.h"
#include "Event.h"

const LPCWSTR winClass = L"Engine";

class Window
{
private:
	HWND m_hwnd;
	int width, height;
	bool isRunning = false;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Window(INT w, INT h) : width(w), height(h), m_hwnd(HWND()) {};
	bool Create(LPCWSTR title);
	bool Runs();
	~Window();
public:
	Event<void> destroy;
	Event<RECT> update;
};

