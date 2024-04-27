#include "pch.h"
#include "App.h"

App::App()
{
	m_Logger = std::make_shared<Logger>();
	m_Graphics = std::make_unique<GraphicsEngine>();
}



void App::Update()
{
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		m_Graphics->Frame();
	}
}

bool App::Runs()
{
	return isRunning;
}

App::~App()
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
