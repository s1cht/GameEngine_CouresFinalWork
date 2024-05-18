#pragma once

#include "pch.h"
#include "Engine/Graphics/GraphicsEngine.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Core/Events/Event.h"
#include "Engine/Core/Logger.h"
#include "Engine/Core/Instance/Instance.h"
#include "Engine/Core/Game/World.h"
#include "Engine/Resources/Camera.h"
#include "lib/ImGUI/imgui.h"
#include "lib/ImGUI/imgui_impl_dx11.h"
#include "lib/ImGUI/imgui_impl_dx9.h"
#include "lib/ImGUI/imgui_impl_win32.h"
//#include "Engine/Core/Instance/Instances/Camera.h"

class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	bool Initialize();
	void Shutdown();

	void Run();
	bool Runs();

	World* GetWorld();
	InstanceService* GetInstanceService();

	Texture* GetTexture(std::string);
	Mesh* GetMesh(std::string);
	Shader* GetShader(std::string);

private:
#ifdef SL_ENGINE_EDITOR
	DWORD RenderEditor();
#endif
	DWORD Render();
	DWORD HandleMessages();

private:
	std::unique_ptr<GraphicsEngine> m_Graphics;
	std::unique_ptr<ResourceManager> m_ResourceManager;
	std::unique_ptr<World> m_World;
	std::unique_ptr<InstanceService> m_InstanceService;

private:
	HANDLE m_GameFinished;
	HANDLE m_EditorFinished;
	bool isRunning = false;
	Vector2 m_windowSize;
};


