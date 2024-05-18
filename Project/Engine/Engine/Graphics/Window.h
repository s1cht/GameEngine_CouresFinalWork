#pragma once
#include "pch.h"
#include "Engine/Core/Events/Event.h"
#include "Engine/Core/Math/EngineMath.h"
#include "lib/ImGUI/imgui.h"
#include "lib/ImGUI/imgui_impl_dx11.h"
#include "lib/ImGUI/imgui_impl_win32.h"

class Window
{
public:
	Window(Vector2 size);
	//Window(const Window&);
	~Window();
#ifndef SL_ENGINE_EDITOR
	bool Initialize(LPCWSTR title, HWND&);
#else
	bool Initialize(LPCWSTR title, HWND&, HWND&);
#endif
	bool Runs();

	Vector2 GetSize();
#ifdef SL_ENGINE_EDITOR
	Vector2 GetRenderSize();
#endif


public:
	Event<void> destroy;
	Event<RECT> update;

private:
	HWND m_hwnd;
	Vector2 m_size;
	bool isRunning = false;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#ifdef SL_ENGINE_EDITOR
	HWND m_hwndRender;
	Vector2 m_sizeRender;
	static LRESULT CALLBACK RenderWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
};

