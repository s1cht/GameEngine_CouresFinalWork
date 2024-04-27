#pragma once

#include "pch.h"
#include "Engine/Graphics/GraphicsEngine.h"
#include "Engine/CoreEvents.h"
#include "Engine/Logger.h"

class App
{
public:
	App();
	~App();

	bool Initialize();
	void Shutdown();

	void Update();
	bool Runs(); 

private:
	//std::unique_ptr<Window> m_Window;
	std::shared_ptr<Logger> m_Logger;
	std::unique_ptr<GraphicsEngine> m_Graphics;

private:
	bool isRunning = false;
};


