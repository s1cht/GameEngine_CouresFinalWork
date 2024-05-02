#include "pch.h"
#include "App.h"

App::App()
{
	m_Engine = nullptr;
}

App::~App()
{
}

void App::Run()
{
	m_Engine->Run();

	return;
}

bool App::Runs()
{
	return isRunning;
}

bool App::Initialize()
{
	if (!m_Engine->Initialize())
	{
		return false;
	}

	return true;
}

void App::Shutdown()
{
	m_Engine->Shutdown();
}
