#pragma once
#include "pch.h"
#include "Event.h"

const LPCWSTR winClass = L"Engine";

class Window
{
public:
	Window();
	Window(const Window&);
	~Window();

	bool Initialize(LPCWSTR title);

	bool Runs();

public:
	Event<void> destroy;
	Event<RECT> update;

private:
	HWND m_hwnd;
	int width, height;
	bool isRunning = false;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

};

