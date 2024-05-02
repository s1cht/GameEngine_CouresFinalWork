#pragma once
#include "pch.h"
#include "Render.h"
#include "Window.h"
#include "Engine/Resources/Camera.h"
#include "Engine/Resources/Mesh.h"
#include "Engine/Resources/Light.h"
#include "Engine/Resources/Shader.h"
#include "Engine/Core/Math/EngineMath.h"

const BOOL VSYNC_ENABLED = true;
const BOOL FULLSCREEN = false;
const FLOAT SCREEN_DEPTH = 1000.f;
const FLOAT SCREEN_NEAR = 0.3f;
const Vector2 WINDOW_SIZE = { 800.f, 600.f };

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	bool Initialize();
	void Shutdown();

	bool Frame(Mesh***, Shader***, INT, INT);

	HWND GetHWND();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

private:
	bool Render(FLOAT, Mesh***, Shader***, INT, INT);

private:
	HWND m_hwnd;
	std::unique_ptr<RenderClass> m_Render;
	std::unique_ptr<Window> m_Window;
	std::shared_ptr<Camera> m_Camera;
	std::shared_ptr<Light> m_Light;
};