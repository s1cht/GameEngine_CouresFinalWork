#pragma once
#include "pch.h"
#include "Render.h"
#include "Window.h"

const BOOL VSYNC_ENABLED = true;
const BOOL FULLSCREEN = false;
const FLOAT SCREEN_DEPTH = 1000.f;
const FLOAT SCREEN_NEAR = 0.3f;

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	bool Initialize();
	void Shutdown();

	bool Frame();

private:
	bool Render();

private:
	HWND m_hwnd;
	std::unique_ptr<RenderClass> m_Render;
	std::unique_ptr<Window> m_Window;
};