#include "pch.h"
#include "GameEngine.h"
#include "Engine/Core/Events/CoreEvents.h"

GameEngine::GameEngine()
{
	m_Graphics = std::make_unique<GraphicsEngine>();
	m_ResourceManager = std::make_unique<ResourceManager>();
	m_World = std::make_unique<World>();
}

bool GameEngine::Initialize()
{
	bool result;

	isRunning = true;

	result = m_ResourceManager->Initialize(101, 106);
	if (!result)
	{
		isRunning = false;
		return false;
	}

	result = m_Graphics->Initialize();
	if (!result)
	{
		isRunning = false;
		return false;
	}

	result = m_ResourceManager->LoadTexture(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), "stone01", m_ResourceManager->GetLoadedStringByID(TEXTURE_STONE01).c_str());
	if (!result)
	{
		isRunning = false;
		return false;
	}

	result = m_ResourceManager->LoadTexture(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), "dirt01", m_ResourceManager->GetLoadedStringByID(TEXTURE_DIRT01).c_str());
	if (!result)
	{
		isRunning = false;
		return false;
	}

	result = m_ResourceManager->LoadObject(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), "DefaultCube", m_ResourceManager->GetLoadedStringByID(MESH_DEFAULTCUBE).c_str());
	if (!result)
	{
		isRunning = false;
		return false;
	}

	result = m_ResourceManager->LoadObject(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), "Sphere", m_ResourceManager->GetLoadedStringByID(MESH_SPHERE).c_str());
	if (!result)
	{
		isRunning = false;
		return false;
	}

	result = m_ResourceManager->LoadShader(m_Graphics->GetDevice(), m_Graphics->GetHWND(), "shader", m_ResourceManager->GetLoadedStringByID(SHADER_VERTEX).c_str(), m_ResourceManager->GetLoadedStringByID(SHADER_PIXEL).c_str());
	if (!result)
	{
		isRunning = false;
		return false;
	}

	return true;
}

void GameEngine::Run()
{
	MSG msg;

	ZeroMemory(&msg, sizeof(msg));

	EngineCoreEvents->AddListener([&](void) { isRunning = false; }, "WindowDestroyed");

	UINT shaderCount;

	Shader** shaders;
	World* world = dynamic_cast<World*>(m_World.get());

	shaders = m_ResourceManager->GetShaders(shaderCount);

	Part* part = dynamic_cast<Part*>(m_World->operator[](L"Part"));
	Light* sunLight = dynamic_cast<Light*>(m_World->operator[](L"SunLight"));
	float rotation = 0.f;
	float rez = 0.005f;
	float angle = 0.f;

	Vector3 vector = Vector3::Zero();


	while (isRunning)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		part->SetRotation(0, rotation, 0);
		sunLight->SetDirection(vector);
		rotation += 0.0005f;
		angle += rez;

		if (angle > 90.f)
			rez = -0.0005f;
		else if (angle < -90.f)
			rez = 0.0005f;

		vector.X = cos(angle);
		vector.Y = 0.f;
		vector.Z = sin(angle);

		m_Graphics->Frame(world, shaders, world->GetChildren().size(), shaderCount);
	}

	return;
}

bool GameEngine::Runs()
{
	return isRunning;
}

World* GameEngine::GetWorld()
{
	return m_World.get();
}

Texture* GameEngine::GetTexture(std::string resource)
{
	return m_ResourceManager->GetTexture(resource);
}

Mesh* GameEngine::GetMesh(std::string resource)
{
	return m_ResourceManager->GetMesh(resource);
}

Shader* GameEngine::GetShader(std::string resource)
{
	return m_ResourceManager->GetShader(resource);
}

GameEngine::~GameEngine()
{
}

void GameEngine::Shutdown()
{
	m_ResourceManager->Shutdown();
	m_Graphics->Shutdown();
}
