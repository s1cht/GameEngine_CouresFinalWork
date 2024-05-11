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

	if (!m_Engine->Initialize())
	{
		return false;
	}

	gameWorld = m_Engine->GetWorld();

	Light* sunLight = Instance::New<Light>(gameWorld);
	sunLight->SetAmbientColor(Color4{ 0.15f, 0.15f,0.15f ,1.f });
	sunLight->SetDiffuseColor(Color4{ 1.f, 1.f , 1.f, 1.f });
	sunLight->SetDirection(Vector3{ 1.f,0.f,0.f });
	sunLight->SetSpecularColor(Color4{ 1.f, 1.f, 1.f, 1.f });
	sunLight->SetSpecularPower(32.f);

	Camera* camera = Instance::New<Camera>(gameWorld);
	camera->SetPosition(Vector3{ -3.f, 0.f, -3.f });
	camera->SetRotation(Vector3{ 0.f, 15.f, 0.f});

	Part* part = Instance::New<Part>(gameWorld);
	part->SetMesh(m_Engine->GetMesh("DefaultCube"));
	part->SetSize(Vector3{ 1.f, 1.f, 1.f });
	part->SetRotation(Vector3{ 0.f, 45.f, 0.f });

	Part* part1 = Instance::New<Part>(gameWorld);
	part1->SetMesh(m_Engine->GetMesh("Sphere"));
	part1->SetSize(Vector3{ 1.f, 1.f, 1.f });
	part1->SetRotation(Vector3{ 0.f, 45.f, 0.f });
	part1->SetPosition(Vector3{ 0.f, 3.f, 0.f });
	part1->SetColor(Color4(0.5f, 0.9f, 0.5f));
	part1->SetTexture(m_Engine->GetTexture("dirt01"));

	Part* part2 = Instance::New<Part>(gameWorld);
	part2->SetMesh(m_Engine->GetMesh("DefaultCube"));
	part2->SetSize(Vector3{ 100.f, 1.f, 100.f });
	part2->SetPosition(Vector3{ 0.f, -0.5f, 0.f });
	part2->SetRotation(Vector3{ 0.f, 0.f, 0.f });
	part2->SetColor(Color4(0.5f, 0.9f, 0.5f));
	part2->SetTexture(m_Engine->GetTexture("dirt01"));

	return true;
}

void App::Shutdown()
{
	m_Engine->Shutdown();
}
