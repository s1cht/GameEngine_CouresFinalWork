#pragma once
#include "pch.h"
#include "Engine/Event.h"
#include "Engine/Math/EngineMath.h"

const LPCWSTR winClass = L"EngineA‡Î‡Ê";

class Window
{
public:
	Window(Vector2 size);
	//Window(const Window&);
	~Window();

	bool Initialize(LPCWSTR title, HWND&);
	bool Runs();

	Vector2 GetSize();

public:
	Event<void> destroy;
	Event<RECT> update;

private:
	HWND m_hwnd;
	Vector2 m_size;
	bool isRunning = false;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

};

