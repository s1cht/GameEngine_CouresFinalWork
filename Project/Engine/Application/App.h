#pragma once

#include "pch.h"
#include "Engine/Graphics/GraphicsEngine.h"
#include "Engine/Core/Events/CoreEvents.h"
#include "Engine/Core/Logger.h"

class App
{
public:
	App();
	~App();

	bool Initialize();
	void Shutdown();

	void Run();
	bool Runs(); 

private:
	//std::unique_ptr<Window> m_Window;
	std::shared_ptr<Logger> m_Logger;
	std::unique_ptr<GraphicsEngine> m_Graphics;

private:
	bool isRunning = false;
};


