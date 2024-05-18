#pragma once
#include "pch.h"
#include "Render.h"
#include "EditorRender.h"
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

	bool Frame(World*&, Shader**&, INT, INT);
#ifdef SL_ENGINE_EDITOR
	bool UpdateEditor(INT, INT);
	bool EditorFrame();
#endif

	HWND GetHWND() const;
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
#ifdef SL_ENGINE_EDITOR
	IDirect3DDevice9* GetEditorDevice();
#endif

private:
	bool Render(World*&, Shader**&, INT, INT);

private:
	HWND m_hwnd;
#ifdef SL_ENGINE_EDITOR
	HWND m_hwndRender;
	std::unique_ptr<EditorRender> m_EditorRender;
#endif

	std::unique_ptr<RenderClass> m_Render;
	std::unique_ptr<Window> m_Window;
};