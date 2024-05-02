#include "pch.h"
#include "GameEngine.h"
#include "Engine/Core/Events/CoreEvents.h"

GameEngine::GameEngine()
{
	m_Graphics = std::make_unique<GraphicsEngine>();
	m_ResourceManager = std::make_unique<ResourceManager>();
}

void GameEngine::Run()
{
	MSG msg;

	ZeroMemory(&msg, sizeof(msg));

	EngineCoreEvents->AddListener([&](void) { isRunning = false; }, "WindowDestroyed");

	UINT shaderCount;
	UINT meshCount;

	Shader** shaders;
	Mesh** meshes;

	shaders = m_ResourceManager->GetShaders(shaderCount);
	meshes = m_ResourceManager->GetMeshes(meshCount);

	while (isRunning)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		m_Graphics->Frame(&meshes, &shaders, meshCount, shaderCount);
	}

	return;
}

bool GameEngine::Runs()
{
	return isRunning;
}

GameEngine::~GameEngine()
{
}

bool GameEngine::Initialize()
{
	bool result;
	isRunning = true;


	result = m_ResourceManager->Initialize();
	if (!result)
	{
		isRunning = false;
		return false;
	}

	result = m_Graphics->Initialize();
	if (!result)
	{
		isRunning = false;
		return false;
	}

	result = m_ResourceManager->LoadShader(m_Graphics->GetDevice(), m_Graphics->GetHWND(), "shader", Shaders[VERTEX_SHADER].c_str(), Shaders[PIXEL_SHADER].c_str());
	if (!result)
	{
		isRunning = false;
		return false;
	}

	result = m_ResourceManager->LoadTexture(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), "stone01", Textures[STONE01].c_str());
	if (!result)
	{
		isRunning = false;
		return false;
	}

	result = m_ResourceManager->LoadTexture(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), "dirt01", Textures[DIRT01].c_str());
	if (!result)
	{
		isRunning = false;
		return false;
	}

	Texture* tex = m_ResourceManager->GetTexture("stone01");

	result = m_ResourceManager->LoadObject(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), &tex, "DefaultCube", Meshes[DEFAULT_CUBE].c_str());
	if (!result)
	{
		isRunning = false;
		return false;
	}

	//EngineCoreEvents->AddListener([&]() { isRunning = false; }, "WindowDestroyed");

	return true;
}

void GameEngine::Shutdown()
{
	m_Graphics->Shutdown();
	m_ResourceManager->Shutdown();
}
