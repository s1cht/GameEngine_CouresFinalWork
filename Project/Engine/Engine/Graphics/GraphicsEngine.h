#pragma once
#include "pch.h"
#include "Render.h"
#include "Window.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"
#include "Engine/Math/EngineMath.h"

const BOOL VSYNC_ENABLED = true;
const BOOL FULLSCREEN = false;
const FLOAT SCREEN_DEPTH = 1000.f;
const FLOAT SCREEN_NEAR = 0.01f;
const Vector2 WINDOW_SIZE = { 800.f, 600.f };

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
	std::unique_ptr<ColorShader> m_ColorShader;
	std::unique_ptr<Window> m_Window;
	std::shared_ptr<ModelClass> m_Model;
	std::shared_ptr<Camera> m_Camera;
};