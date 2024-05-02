#include "pch.h"
#include "App.h"

App::App()
{
	m_ = std::make_unique<GraphicsEngine>();
}



void App::Run()
{
	MSG msg;
	
	ZeroMemory(&msg, sizeof(msg));
	
	EngineCoreEvents->AddListener([&](void) { isRunning = false; }, "WindowDestroyed");

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

bool App::Runs()
{
	return isRunning;
}

App::~GameEngine()
{
}

bool App::Initialize()
{
	isRunning = true;
	if (!m_Graphics->Initialize())
	{
		isRunning = false;
		return false;
	}

	EngineCoreEvents->AddListener([&]() { isRunning = false; }, "WindowDestroyed");

	return true;
}

void App::Shutdown()
{
	m_Graphics->Shutdown();
}
