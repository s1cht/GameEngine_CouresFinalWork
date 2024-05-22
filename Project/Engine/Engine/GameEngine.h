#pragma once

#include "pch.h"
#include "Engine/Graphics/GraphicsEngine.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Resources/FileManager.h"
#include "Engine/Core/Events/Event.h"
#include "Engine/Core/Logger.h"
#include "Engine/Core/Instance/Instance.h"
#include "Engine/Core/Instance/Instances/Camera.h"
#include "Engine/Core/Instance/IUnknown.h"
#include "Engine/Core/Game/World.h"
#include "Engine/Editor/EditorDesigner.h"
#include "Keyboard.h"
#include "Mouse.h"
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

	ENG_PTR<World> GetWorld();
	InstanceService* GetInstanceService();

	Texture* GetTexture(std::string);
	Mesh* GetMesh(std::string);
	Shader* GetShader(std::string);

private:
	DWORD Render();

	bool ReadProjectFile();
	bool SaveProjectFile();
	bool CreateProjectFile();
	bool CloseProjectFile();

private:
	std::unique_ptr<GraphicsEngine> m_Graphics;
	std::unique_ptr<ResourceManager> m_ResourceManager;
	ENG_PTR<World> m_World;
	std::unique_ptr<InstanceService> m_InstanceService;
	std::unique_ptr<Keyboard> m_Keyboard;
	std::unique_ptr<Mouse> m_Mouse;
	std::unique_ptr<FileManager> m_FileManager;
#ifdef SL_ENGINE_EDITOR
	std::unique_ptr<EditorDesigner> m_EditorDesigner;
#endif

private:
	bool isProjectLoaded = false;
	bool isRunning = false;
	Vector2 m_windowSize;
	std::wstring projectName;

};


