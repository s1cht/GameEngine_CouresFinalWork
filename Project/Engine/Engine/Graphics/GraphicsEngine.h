#pragma once
#include "pch.h"
#include "Render.h"
#include "RenderToTexture.h"
#include "Window.h"
#include "Engine/Core/Events/CoreEvents.h"
#include "Engine/Resources/Mesh.h"
#include "Engine/Resources/Shader.h"
#include "Engine/Core/Math/EngineMath.h"
#include "Engine/Core/Instance/Instance.h"
#include "Engine/Core/Game/World.h"
#include "Engine/Core/Instance/Instances/Camera.h"
#include "Engine/Core/Instance/Instances/Light.h"
#include "Engine/Core/Instance/Instances/Part.h"
#include "Keyboard.h"
#include "Mouse.h"

const BOOL VSYNC_ENABLED = false;
const BOOL FULLSCREEN = false;
const FLOAT SCREEN_DEPTH = 5000.f;
const FLOAT SCREEN_NEAR = 0.3f;
const Vector2 WINDOW_SIZE = { 1920.f, 1080.f };
#ifndef SL_ENGINE_EDITOR
const LPCWSTR WINDOW_NAME = L"Game";
#else
const LPCWSTR WINDOW_NAME = L"SL Engine";
#endif


class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	bool Initialize();
	void Shutdown();

	bool Frame(ENG_PTR<World>, Shader**&, INT, INT, FLOAT, FLOAT
#ifdef SL_ENGINE_EDITOR
		, std::function<void(ID3D11ShaderResourceView*)>&, bool
#endif
	);

	HWND GetHWND() const;
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11ShaderResourceView* GetRenderResource();

private:
	bool Render(ENG_PTR<World>, Shader**&, INT, INT, FLOAT, FLOAT
#ifdef SL_ENGINE_EDITOR
		, std::function<void(ID3D11ShaderResourceView*)>& editorOutput, bool
#endif
	);

private:
	HWND m_hwnd;
	bool m_windowResized;
	Vector2 m_newSize;

	std::unique_ptr<RenderClass> m_Render;
	std::unique_ptr<RenderToTexture> m_RenderToTexture;
	std::unique_ptr<Window> m_Window;
};