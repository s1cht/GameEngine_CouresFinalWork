#include "pch.h"
#include "App.h"

App::App()
{
	m_Window = std::make_unique<Window>(1000, 1000);

	if (!m_Window->Create(L"First program!"))
		isRunning = false;
	else
		isRunning = true;
}

void App::Update()
{
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

bool App::Runs()
{
	return m_Window->Runs();
}

App::~App()
{
}