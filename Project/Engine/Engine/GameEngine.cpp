#include "pch.h"
#include "GameEngine.h"
#include "Engine/Core/Events/CoreEvents.h"

GameEngine::GameEngine()
{
	m_Graphics = std::make_unique<GraphicsEngine>();
	m_ResourceManager = std::make_unique<ResourceManager>();
	m_World = TO_ENG_PTR<World>();
	m_InstanceService = std::make_unique<InstanceService>();
	m_windowSize = Vector2::Zero();
	m_FileManager = std::make_unique<FileManager>();
	m_Keyboard = std::make_unique<Keyboard>();
	m_Mouse = std::make_unique<Mouse>();
}

bool GameEngine::Initialize()
{
	bool result;

	isRunning = true;

	result = m_ResourceManager->Initialize(101, 106);
	if (!result)
	{
		MessageBox(NULL, L"Resource loading", L"Error", MB_OK);
		isRunning = false;
		return false;
	}

	result = m_Graphics->Initialize();
	if (!result)
	{
		MessageBox(NULL, L"Graphics loading", L"Error", MB_OK);
		isRunning = false;
		return false;
	}

	result = m_ResourceManager->LoadTexture(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), "stone01", m_ResourceManager->GetLoadedStringByID(TEXTURE_STONE01).c_str());
	if (!result)
	{
		MessageBox(NULL, L"Texture loading", L"Error", MB_OK);
		isRunning = false;
		return false;
	}

	result = m_ResourceManager->LoadTexture(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), "dirt01", m_ResourceManager->GetLoadedStringByID(TEXTURE_DIRT01).c_str());
	if (!result)
	{
		MessageBox(NULL, L"Texture loading", L"Error", MB_OK);
		isRunning = false;
		return false;
	}

	result = m_ResourceManager->LoadObject(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), "DefaultCube", m_ResourceManager->GetLoadedStringByID(MESH_DEFAULTCUBE).c_str());
	if (!result)
	{
		MessageBox(NULL, L"Object loading", L"Error", MB_OK);
		isRunning = false;
		return false;
	}

	result = m_ResourceManager->LoadObject(m_Graphics->GetDevice(), m_Graphics->GetDeviceContext(), "Sphere", m_ResourceManager->GetLoadedStringByID(MESH_SPHERE).c_str());
	if (!result)
	{
		MessageBox(NULL, L"Object loading", L"Error", MB_OK);
		isRunning = false;
		return false;
	}

	result = m_ResourceManager->LoadShader(m_Graphics->GetDevice(), m_Graphics->GetHWND(), "shader", m_ResourceManager->GetLoadedStringByID(SHADER_VERTEX).c_str(), m_ResourceManager->GetLoadedStringByID(SHADER_PIXEL).c_str());
	if (!result)
	{
		MessageBox(NULL, L"Shader loading", L"Error", MB_OK);
		isRunning = false;
		return false;
	}

#ifdef SL_ENGINE_EDITOR
	m_EditorDesigner = std::make_unique<EditorDesigner>();
	result = m_EditorDesigner->Initialize(m_Graphics->GetHWND(), m_Graphics->GetDevice(), m_Graphics->GetDeviceContext());
	if (!result)
	{
		isRunning = false;
		return false;
	}
#endif

	m_InstanceService->SetDevice(m_Graphics->GetDevice());
	m_InstanceService->SetDeviceContext(m_Graphics->GetDeviceContext());

	return true;
}

void GameEngine::Run()
{
	MSG msg;
	std::thread renderThread;

	EngineCoreEvents->AddListener([&](void) { isRunning = false; }, "WindowDestroyed");
	EngineCoreEvents->AddListener<Vector2>([&](EVENT_RET_TYPE<Vector2> newSize) {m_windowSize.X = newSize->X; m_windowSize.Y = newSize->Y; }, "WindowUpdated");

	renderThread = std::thread(&GameEngine::Render, this);

	ZeroMemory(&msg, sizeof(msg));

	while (isRunning)
	{
		if (PeekMessage(&msg, m_Graphics->GetHWND(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if (renderThread.joinable()) {
		renderThread.join();
	}

	return;
}

bool GameEngine::Runs()
{
	return isRunning;
}

ENG_PTR<World> GameEngine::GetWorld()
{
	return m_World;
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

bool GameEngine::ReadProjectFile()
{
	wifstream file;
	std::wstring fileName;
	std::wstring buff;
	std::wstring buff1;
	Vector3 buffVec3;
	//Vector2 buffVec2;
	Color4 buffClr4;
	INT buffInt;
	INT buffInt1;
	INT unknown = 0;
	bool endFile = false;

	fileName = m_FileManager->GetFileName();
	if (fileName.empty())
		return false;

	file.open(fileName);
	if (!file.is_open())
		return false;

	SLPR::WaitUntilEqual(file);

	std::getline(file, projectName, SLPR_CHAR_BREAK);

	while (!endFile)
	{
		// Read type
		SLPR::WaitUntilEqual(file);
		getline(file, buff, SLPR_CHAR_BREAK);

		//Read id
		SLPR::WaitUntilEqual(file);
		file >> buffInt;

		//Read name
		SLPR::WaitUntilEqual(file);
		getline(file, buff1, SLPR_CHAR_BREAK);

		//Read parent
		SLPR::WaitUntilEqual(file);
		file >> buffInt1;

		if (buff == L"World")
		{
			m_World = TO_ENG_PTR<World>();
			m_World->SetID(buffInt);
		}
		else if (buff == L"Part")
		{
			INSTANCE parent = m_World->GetChildByID(buffInt1);
			ENG_PTR<Part> part = m_InstanceService->New<Part>(m_World, buffInt);
			if (parent)
				InstanceService::SetParent(parent, part);
			part->SetName(buff1);

			//Read position
			SLPR::WaitUntilEqual(file);
			file >> buffVec3.X >> buffVec3.Y >> buffVec3.Z;

			part->SetPosition(buffVec3);

			//Read rotation
			SLPR::WaitUntilEqual(file);
			file >> buffVec3.X >> buffVec3.Y >> buffVec3.Z;

			part->SetRotation(buffVec3);

			//Read size
			SLPR::WaitUntilEqual(file);
			file >> buffVec3.X >> buffVec3.Y >> buffVec3.Z;

			part->SetSize(buffVec3);

			//Read color
			SLPR::WaitUntilEqual(file);
			file >> buffClr4.R >> buffClr4.G >> buffClr4.B >> buffClr4.A;

			part->SetColor(buffClr4);

			//Read mesh
			SLPR::WaitUntilEqual(file);
			getline(file, buff1, SLPR_CHAR_BREAK);

			part->SetMesh(m_ResourceManager->GetMesh((wstring_to_utf8(buff1))));

			//Read texture
			SLPR::WaitUntilEqual(file);
			getline(file, buff1, SLPR_CHAR_BREAK);

			part->SetTexture(GetTexture(wstring_to_utf8(buff1)));
		}
		else
		{
			INSTANCE parent = m_World->GetChildByID(buffInt1);
			ENG_PTR<UnknownInstance> unknownInst = m_InstanceService->New<UnknownInstance>(m_World, buffInt);
			if (parent)
				InstanceService::SetParent(parent, unknownInst);
			unknown++;
		}

		SLPR::WaitUntilReserved(file, endFile);
	}

	ENG_PTR<Light> sunLight = m_InstanceService->New<Light>(m_World);
	sunLight->SetAmbientColor(Color4{ 0.15f, 0.15f,0.15f ,1.f });
	sunLight->SetDiffuseColor(Color4{ 1.f, 1.f , 1.f, 1.f });
	sunLight->SetDirection(Vector3{ 1.f,0.f,0.f });
	sunLight->SetSpecularColor(Color4{ 1.f, 1.f, 1.f, 1.f });
	sunLight->SetSpecularPower(32.f);

	ENG_PTR<Camera> camera = m_InstanceService->New<Camera>(m_World);
	camera->SetPosition(Vector3{ -3.f, 0.f, -3.f });
	camera->SetRotation(Vector3{ 0.f, 15.f, 0.f });

	isProjectLoaded = true;
	m_EditorDesigner->SetProjectLoaded(isProjectLoaded);

	return true;
}

bool GameEngine::SaveProjectFile()
{
	wofstream file;
	std::wstring fileName;
	bool finished = false;

	fileName = m_FileManager->GetFileName();
	if (fileName.empty())
		return false;

	file.open(fileName);
	if (!file.is_open())
		return false;

	SLPR::AddValueLine(file, SLPR_STR_PROJECT_NAME, projectName);
	SLPR::AddEmptyLine(file);

	// World
	SLPR::AddValueLine(file, SLPR_STR_TYPE, InstanceService::ConvertClassNameToWString(m_World->GetInstanceClassName()));
	SLPR::AddValueLine(file, SLPR_STR_ID, 0);
	SLPR::AddValueLine(file, SLPR_STR_NAME, m_World->GetName());
	SLPR::AddValueLine(file, SLPR_STR_PARENT, 0);

	for (size_t i = 1; i <= (size_t)m_InstanceService->GetInstanceCount(); i++)
	{
		INSTANCE inst = m_World->GetChildByID(i);

		if (!inst)
		{
			SLPR::AddEmptyLine(file);
			SLPR::AddReservedLine(file, SLPR_STR_RESERVED_WORD_END);
			return false;
		}

		SLPR::AddEmptyLine(file);
		SLPR::AddReservedLine(file, SLPR_STR_RESERVED_WORD_NEW);
		SLPR::AddEmptyLine(file);

		SLPR::AddValueLine(file, SLPR_STR_TYPE, InstanceService::ConvertClassNameToWString(inst->GetInstanceClassName()));
		SLPR::AddValueLine(file, SLPR_STR_ID, inst->GetID());
		SLPR::AddValueLine(file, SLPR_STR_NAME, inst->GetName());
		SLPR::AddValueLine(file, SLPR_STR_PARENT, inst->GetParent()->GetID());

		switch (inst->GetInstanceClassName())
		{
		case PART:
		{

			Part* part = dynamic_cast<Part*>(inst.get());
			SLPR::AddValueLine(file, SLPR_STR_POSITION, part->GetPosition().X, part->GetPosition().Y, part->GetPosition().Z);
			SLPR::AddValueLine(file, SLPR_STR_ROTATION, part->GetRotation().X, part->GetRotation().Y, part->GetRotation().Z);
			SLPR::AddValueLine(file, SLPR_STR_SIZE, part->GetSize().X, part->GetSize().Y, part->GetSize().Z);
			SLPR::AddValueLine(file, SLPR_STR_COLOR, part->GetColor().R, part->GetColor().G, part->GetColor().B, part->GetColor().A);
			SLPR::AddValueLine(file, SLPR_STR_MESH, utf8_to_wstring(part->GetMesh()->GetName()));

			if (part->GetTexture())
				SLPR::AddValueLine(file, SLPR_STR_TEXTURE, utf8_to_wstring(part->GetTexture()->GetName()));
			else
				SLPR::AddValueLine(file, SLPR_STR_TEXTURE, SLPR_STR_NOTHING);

			break;
		}
		default:
			break;
		}
	}

	SLPR::AddEmptyLine(file);
	SLPR::AddReservedLine(file, SLPR_STR_RESERVED_WORD_END);

	return true;
}

bool GameEngine::CreateProjectFile()
{
	wofstream file;
	std::wstring fileName;
	bool finished = false;

	fileName = m_FileManager->GetFileName();
	if (fileName.empty())
		return false;

	file.open(fileName);
	if (!file.is_open())
		return false;

	SLPR::AddValueLine(file, SLPR_STR_PROJECT_NAME, projectName);
	SLPR::AddEmptyLine(file);

	m_World = TO_ENG_PTR<World>();
	m_World->SetID(0);

	// World
	SLPR::AddValueLine(file, SLPR_STR_TYPE, InstanceService::ConvertClassNameToWString(m_World->GetInstanceClassName()));
	SLPR::AddValueLine(file, SLPR_STR_ID, 0);
	SLPR::AddValueLine(file, SLPR_STR_NAME, m_World->GetName());
	SLPR::AddValueLine(file, SLPR_STR_PARENT, 0);

	SLPR::AddEmptyLine(file);
	SLPR::AddReservedLine(file, SLPR_STR_RESERVED_WORD_END);

	return true;
}

bool GameEngine::CloseProjectFile()
{
	return false;
}

DWORD GameEngine::Render()
{
	UINT shaderCount;
	float rez = 0.005f;
	float angle = 0.f;
	Vector3 vector = Vector3::Zero();
	FLOAT pitch = 0.f, yaw = 0.f;
	Shader** shaders;
	Camera* camera;
	Light* light;
	bool focused = false;

#ifdef SL_ENGINE_EDITOR
	INSTANCE selectedInstance = nullptr;

	std::function<void(ENG_PTR<Instance>, INT&, INT&, ImGuiTreeNodeFlags&)> addElement = [&](ENG_PTR<Instance> parent, INT& element, INT& selected, ImGuiTreeNodeFlags& flags)
		{
			for (auto instance : parent->GetChildren())
			{
				flags = ImGuiTreeNodeFlags_DefaultOpen;

				if (selected == element)
				{
					flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Selected;
				}

				if (instance->GetChildren().empty())
				{
					flags |= ImGuiTreeNodeFlags_Leaf;
					if (ImGui::TreeNodeEx(wstring_to_utf8(instance->GetName() + L" " + std::to_wstring(instance->GetID()) + L"##" + std::to_wstring(element)).c_str(), flags))
					{
						if (ImGui::IsItemClicked())
						{
							selected = element;
							EngineCoreEvents->FireEvent("EditorExplorerSelection", instance);
						}

						element++;

						ImGui::TreePop();
					}
				}
				else
				{
					if (ImGui::TreeNodeEx(wstring_to_utf8(instance->GetName() + L" " + std::to_wstring(instance->GetID()) + L"##" + std::to_wstring(element)).c_str(), flags))
					{
						if (ImGui::IsItemClicked())
						{
							selected = element;
							EngineCoreEvents->FireEvent("EditorExplorerSelection", instance);
						}

						element++;

						addElement(instance, element, selected, flags);

						ImGui::TreePop();
					}
				}
			}
		};
	std::function<void(Vector3, Vector3, Vector3)> createPart = [&](Vector3 newPosition, Vector3 newSize, Vector3 newRotation)
		{
			ENG_PTR<Part> part = m_InstanceService->New<Part>(m_World);
			part->SetMesh(GetMesh("DefaultCube"));
			part->SetPosition(newPosition);
			part->SetRotation(newRotation);
			part->SetSize(newSize);
			part->SetName(L"Part");
		};

	EngineCoreEvents->AddListener<Instance>([&](const INSTANCE newInst)
		{ selectedInstance = newInst; 
		}, "EditorExplorerSelection");

	EngineCoreEvents->AddListener([&]()
		{ 
			if (!m_FileManager->CallFileOpenScreen(m_Graphics->GetHWND()))
				return;
			if (!ReadProjectFile())
				return;
			light = dynamic_cast<Light*>(m_World->operator[](L"SunLight").get());
			camera = dynamic_cast<Camera*>(m_World->operator[](L"Camera").get());
		}, 
		"OpenProject");

	EngineCoreEvents->AddListener([&]()
		{
			if (!SaveProjectFile())
				return;
		},
		"SaveProject");

	EngineCoreEvents->AddListener([&]()
		{
			if (!m_FileManager->CallFileSaveScreen(m_Graphics->GetHWND()))
				return;
			if (!SaveProjectFile())
				return;
		},
		"SaveAsProject");

	EngineCoreEvents->AddListener([&]()
		{
			if (!CreateProjectFile())
				return;
			if (!ReadProjectFile())
				return;
			light = dynamic_cast<Light*>(m_World->operator[](L"SunLight").get());
			camera = dynamic_cast<Camera*>(m_World->operator[](L"Camera").get());
		},
		"CreateNewProject");

#endif
	
	shaders = m_ResourceManager->GetShaders(shaderCount);

	while (isRunning)
	{
		//sunLight->SetDirection(vector);
		angle += rez;

		if (angle > 90.f)
			rez = -0.0005f;
		else if (angle < -90.f)
			rez = 0.0005f;

		vector.X = cos(angle);
		vector.Y = 0.f;
		vector.Z = sin(angle);

#ifdef SL_ENGINE_EDITOR


		m_EditorDesigner->NewFrame();

		auto key = m_Keyboard->GetState();

		static float speed = 0.005f;
		static float sens = 0.075f;

		if (isProjectLoaded)
		{
			if (key.Left)
			{
				yaw -= 1.f * sens;
			}
			if (key.Right)
			{
				yaw += 1.f * sens;
			}
			if (key.Up)
			{
				pitch -= 1.f * sens;
			}
			if (key.Down)
			{
				pitch += 1.f * sens;
			}

			if (key.W)
			{
				camera->SetPosition(camera->GetPosition() + camera->GetForwardpVector() * speed);
			}
			if (key.S)
			{
				camera->SetPosition(camera->GetPosition() + camera->GetForwardpVector() * -speed);
			}
			if (key.A)
			{
				camera->SetPosition(camera->GetPosition() + camera->GetRightVector() * -speed);
			}
			if (key.D)
			{
				camera->SetPosition(camera->GetPosition() + camera->GetRightVector() * speed);
			}
			if (key.Space)
			{
				camera->SetPosition(camera->GetPosition() + camera->GetUpVector() * speed);
			}
			if (key.LeftControl)
			{
				camera->SetPosition(camera->GetPosition() + camera->GetUpVector() * -speed);
			}
		}

		ImGui::Begin("sta");
		{
			ImGui::Text("X %.3f", pitch);
			ImGui::Text("Y %.3f", yaw);

			ImGui::End();
		}

		m_EditorDesigner->CreateMainMenuBar();
		m_EditorDesigner->CreateDockingPlace();

		//Explorer
		m_EditorDesigner->CreateExplorer(m_World.get(), addElement);

		//Instance
		m_EditorDesigner->CreateInstanceCreator(createPart);

		m_EditorDesigner->CreateProperties(selectedInstance, m_World, m_EditorDesigner->GetSelectedExplorerItem());

		m_EditorDesigner->CreateProjectSetup(projectName, m_FileManager.get(), m_Graphics->GetHWND());
#endif

		m_Graphics->Frame(m_World, shaders, m_World->GetChildren().size(), shaderCount, pitch, yaw
#ifdef SL_ENGINE_EDITOR
			, m_EditorDesigner->RenderWindow, isProjectLoaded
#endif
		);
	}
	return 1;
}

GameEngine::~GameEngine()
{
}

void GameEngine::Shutdown()
{
	m_ResourceManager->Shutdown();
	m_Graphics->Shutdown();
#ifdef SL_ENGINE_EDITOR
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
}