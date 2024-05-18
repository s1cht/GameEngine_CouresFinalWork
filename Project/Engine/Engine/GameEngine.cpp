#include "pch.h"
#include "GameEngine.h"
#include "Engine/Core/Events/CoreEvents.h"


GameEngine::GameEngine()
{
	m_Graphics = std::make_unique<GraphicsEngine>();
	m_ResourceManager = std::make_unique<ResourceManager>();
	m_World = std::make_unique<World>();
	m_InstanceService = std::make_unique<InstanceService>();
	m_windowSize = Vector2::Zero();
}

bool GameEngine::Initialize()
{
	bool result;

	isRunning = true;

	result = IMGUI_CHECKVERSION();
	if (!result)
	{
		isRunning = false;
		return false;
	}

	result = ImGui::CreateContext();
	if (!result)
	{
		isRunning = false;
		return false;
	}

	ImGui::StyleColorsLight();

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

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(m_Graphics->GetHWND());
	ImGui_ImplDX9_Init(m_Graphics->GetEditorDevice());

	m_InstanceService->SetDevice(m_Graphics->GetDevice());
	m_InstanceService->SetDeviceContext(m_Graphics->GetDeviceContext());

	return true;
}

void GameEngine::Run()
{
	MSG msg;
	std::thread editorThread;
	std::thread gameThread;

	EngineCoreEvents->AddListener([&](void) { isRunning = false; }, "WindowDestroyed");
	EngineCoreEvents->AddListener<Vector2>([&](Vector2 newSize) {m_windowSize = newSize; }, "WindowUpdated");

#ifdef SL_ENGINE_EDITOR
	editorThread = std::thread(&GameEngine::RenderEditor, this);
#endif
	gameThread = std::thread(&GameEngine::Render, this);

	ZeroMemory(&msg, sizeof(msg));

	while (isRunning)
	{
		if (PeekMessage(&msg, m_Graphics->GetHWND(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

#ifdef SL_ENGINE_EDITOR
	if (editorThread.joinable()) {
		editorThread.join();
	}
#endif
	if (gameThread.joinable()) {
		gameThread.join();
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

InstanceService* GameEngine::GetInstanceService()
{
	return m_InstanceService.get();
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

DWORD GameEngine::RenderEditor()
{
	Vector3 f = Vector3::Zero();
	bool show_demo_window = true;
	bool show_another_window = false;
	ImGuiIO& io = ImGui::GetIO();
	int c = 0;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	while (isRunning)
	{
		m_Graphics->UpdateEditor((INT)m_windowSize.X, (INT)m_windowSize.Y);

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		{
			ImGui::Begin("Hello, world!");

			ImGui::Text("This is some useful text.");
			ImGui::Checkbox("Demo Window", &show_demo_window);

			ImGui::SliderFloat("PartX", &f.X, -10.0f, 10.0f);
			ImGui::SliderFloat("PartY", &f.Y, -10.0f, 10.0f);
			ImGui::SliderFloat("PartZ", &f.Z, -10.0f, 10.0f);

			ImGui::Text("Application average (%.1f FPS)", io.Framerate);
			ImGui::End();
		}

		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}
		m_Graphics->EditorFrame();
		
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	return 1;
}

DWORD GameEngine::Render()
{
	UINT shaderCount;
	float rotation = 0.f;
	float rez = 0.005f;
	float angle = 0.f;
	Vector3 vector = Vector3::Zero();
	Shader** shaders;
	World* world = dynamic_cast<World*>(m_World.get());
	Light* sunLight = dynamic_cast<Light*>(m_World->operator[](L"SunLight"));
	int c = 0;

	shaders = m_ResourceManager->GetShaders(shaderCount);

	while (isRunning)
	{
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
	return 1;
}

DWORD GameEngine::HandleMessages()
{

	return 1;
}

GameEngine::~GameEngine()
{
}

void GameEngine::Shutdown()
{
	m_ResourceManager->Shutdown();
	m_Graphics->Shutdown();
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}