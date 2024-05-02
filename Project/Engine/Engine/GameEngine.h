#pragma once

#include "pch.h"
#include "Engine/Graphics/GraphicsEngine.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Core/Events/Event.h"
#include "Engine/Core/Logger.h"


class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	bool Initialize();
	void Shutdown();

	void Run();
	bool Runs();

private:
	std::unique_ptr<GraphicsEngine> m_Graphics;
	std::unique_ptr<ResourceManager> m_ResourceManager;

private:
	bool isRunning = false;
};


