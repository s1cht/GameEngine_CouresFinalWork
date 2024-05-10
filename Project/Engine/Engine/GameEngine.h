#pragma once

#include "pch.h"
#include "Engine/Graphics/GraphicsEngine.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Core/Events/Event.h"
#include "Engine/Core/Logger.h"
#include "Engine/Core/Instance/Instance.h"
#include "Engine/Core/World/World.h"
#include "Engine/Resources/Camera.h"
//#include "Engine/Core/Instance/Instances/Camera.h"


class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	bool Initialize();
	void Shutdown();

	void Run();
	bool Runs();

	World* GetWorld();

	Texture* GetTexture(std::string);
	Mesh* GetMesh(std::string);
	Shader* GetShader(std::string);

private:
	std::unique_ptr<GraphicsEngine> m_Graphics;
	std::unique_ptr<ResourceManager> m_ResourceManager;
	std::unique_ptr<World> m_World;

private:
	bool isRunning = false;
};


