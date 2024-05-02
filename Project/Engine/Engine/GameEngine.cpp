#include "pch.h"
#include "GameEngine.h"

GameEngine::GameEngine()
{
	m_Graphics = std::make_unique<GraphicsEngine>();
	m_ResourceManager = std::make_unique<ResourceManager>();
}



void GameEngine::Run()
{
	MSG msg;

	ZeroMemory(&msg, sizeof(msg));

	//EngineCoreEvents->AddListener([&](void) { isRunning = false; }, "WindowDestroyed");

	while (isRunning)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		m_Graphics->Frame();
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
	isRunning = true;
	if (!m_Graphics->Initialize())
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
}
