#include "pch.h"
#include "App.h"

App::App()
{
	m_Logger = std::make_shared<Logger>();
	m_Graphics = std::make_unique<GraphicsEngine>();

	if (!m_Graphics->Initialize())
		isRunning = false;
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
	return false;
}

App::~App()
{
}

void App::Shutdown()
{
	m_Graphics->Shutdown();
}
