#include "pch.h"
#include "App.h"

App::App()
{
	m_Engine = std::make_unique<GameEngine>();
}

App::~App()
{
}

void App::Run()
{
	m_Engine->Run();

	return;
}

bool App::Initialize()
{
	World* gameWorld;
	InstanceService* instanceService;

	if (!m_Engine->Initialize())
	{
		return false;
	}

	gameWorld = m_Engine->GetWorld();
	instanceService = m_Engine->GetInstanceService();

	Light* sunLight = instanceService->New<Light>(gameWorld);
	sunLight->SetAmbientColor(Color4{ 0.15f, 0.15f,0.15f ,1.f });
	sunLight->SetDiffuseColor(Color4{ 1.f, 1.f , 1.f, 1.f });
	sunLight->SetDirection(Vector3{ 1.f,0.f,0.f });
	sunLight->SetSpecularColor(Color4{ 1.f, 1.f, 1.f, 1.f });
	sunLight->SetSpecularPower(32.f);

	Camera* camera = instanceService->New<Camera>(gameWorld);
	camera->SetPosition(Vector3{ -3.f, 0.f, -3.f });
	camera->SetRotation(Vector3{ 0.f, 15.f, 0.f});

	for (size_t i = 0; i < 10; i++)
	{
		Part* part = instanceService->New<Part>(gameWorld);
		part->SetMesh(m_Engine->GetMesh("Sphere"));
		part->SetSize(Vector3{ 1.f, 1.f, 1.f });
		part->SetRotation(Vector3{ 0.f, 45.f, 0.f });
		part->SetPosition(Vector3{ (float)i - 5.f , 0.f, 0.f });
	}

	gameWorld = nullptr;
	instanceService = nullptr;

	return true;
}

void App::Shutdown()
{
	m_Engine->Shutdown();
}
