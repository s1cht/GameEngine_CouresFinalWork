#pragma once

#include "pch.h"
#include "Engine/GameEngine.h"

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
	std::unique_ptr<GameEngine> m_Engine;

private:
	bool isRunning = false;
};


