#pragma once
#include "pch.h"
#include "Render.h"
#include "Window.h"
#include "Engine/Resources/Camera.h"
#include "Engine/Resources/Mesh.h"
#include "Engine/Resources/Shader.h"
#include "Engine/Core/Math/EngineMath.h"
#include "Engine/Core/Instance/Instance.h"
#include "Engine/Core/Game/World.h"
#include "Engine/Core/Instance/Instances/Light.h"
#include "Engine/Core/Instance/Instances/Part.h"

const BOOL VSYNC_ENABLED = false;
const BOOL FULLSCREEN = false;
const FLOAT SCREEN_DEPTH = 5000.f;
const FLOAT SCREEN_NEAR = 0.3f;
const Vector2 WINDOW_SIZE = { 800.f, 600.f };

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	bool Initialize();
	void Shutdown();

	bool Frame(World*&, Shader**&, INT, INT);

	HWND GetHWND() const;
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

private:
	bool Render(World*&, Shader**&, INT, INT);

private:
	HWND m_hwnd;
	std::unique_ptr<RenderClass> m_Render;
	std::unique_ptr<Window> m_Window;
};