#include "pch.h"
#include "EditorDesigner.h"

std::string wstring_to_utf8(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();
	INT size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

std::wstring utf8_to_wstring(const std::string& str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

#ifdef SL_ENGINE_EDITOR

EditorDesigner::EditorDesigner()
{
	m_io = nullptr;

	m_mainViewport = nullptr;

	m_explorerActive = false;

	m_instanceCreatorActive = false;

	m_ic_position = Vector3::Zero();
	m_ic_posArray[0] = 0.f;
	m_ic_posArray[1] = 0.f;
	m_ic_posArray[2] = 0.f;

	m_ic_rotation = Vector3::Zero();
	m_ic_rotArray[0] = 0.f;
	m_ic_rotArray[1] = 0.f;
	m_ic_rotArray[2] = 0.f;

	m_ic_size = Vector3(1.f, 1.f, 1.f);
	m_ic_sizeArray[0] = 1.f;
	m_ic_sizeArray[1] = 1.f;
	m_ic_sizeArray[2] = 1.f;

	m_r_imagePosition = ImVec2(0, 0);
	m_r_imageSize = ImVec2(0, 0);
	m_r_windowRegionSize = ImVec2(0, 0);

	m_pr_nameBuffer = new char[32];

	m_mmb_menuBarSize = ImVec2(0, 0);

	m_pr_selected = -1;

	m_projectSetup = false;
}

EditorDesigner::~EditorDesigner()
{
}

bool EditorDesigner::Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	bool result;

	result = IMGUI_CHECKVERSION();
	if (!result)
		return false;

	result = ImGui::CreateContext();
	if (!result)
		return false;

	ImGui::StyleColorsDark();

	result = ImGui_ImplWin32_Init(hwnd);
	if (!result)
		return false;
	result = ImGui_ImplDX11_Init(device, deviceContext);
	if (!result)
		return false;

	m_io						= &ImGui::GetIO();

	m_io->ConfigFlags			|= ImGuiConfigFlags_NavEnableKeyboard;
	m_io->ConfigFlags			|= ImGuiConfigFlags_DockingEnable;

	m_mainViewport				= ImGui::GetMainViewport();

	RenderWindow = [&](ID3D11ShaderResourceView* renderResource)
		{
			if (!m_isProjectLoaded) return;
			ImGui::Begin("Render", nullptr, WindowFlags::RENDER_WINDOW_FLAGS);

			m_r_windowRegionSize = ImGui::GetWindowContentRegionMax();

			if (m_r_windowRegionSize.x * WindowFlags::ASPECT_RATIO < m_r_windowRegionSize.y)
				m_r_imageSize = ImVec2(ImGui::GetWindowContentRegionMax().x * 0.95f, (ImGui::GetWindowContentRegionMax().x * WindowFlags::ASPECT_RATIO) * 0.95f);
			else
				m_r_imageSize = ImVec2((ImGui::GetWindowContentRegionMax().y / WindowFlags::ASPECT_RATIO) * 0.95f, ImGui::GetWindowContentRegionMax().y * 0.95f);

			m_r_imagePosition = ImVec2((ImGui::GetWindowSize().x - m_r_imageSize.x) * 0.5f, (ImGui::GetWindowSize().y - m_r_imageSize.y) * 0.5f);

			ImGui::SetWindowPos(ImVec2(0, m_mmb_menuBarSize.y), ImGuiCond_Once);
			ImGui::SetWindowSize(ImVec2(m_io->DisplaySize.x, m_io->DisplaySize.y - m_mmb_menuBarSize.y), ImGuiCond_Once);

			ImGui::SetCursorPos(m_r_imagePosition);
			ImGui::Image(renderResource, m_r_imageSize);

			ImGui::End();
		};

	return true;
}

void EditorDesigner::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	delete[] m_pr_nameBuffer;
}

void EditorDesigner::SetProjectLoaded(bool loaded)
{
	m_isProjectLoaded = loaded;
}

INT EditorDesigner::GetSelectedExplorerItem()
{
	return m_pr_selected;
}

void EditorDesigner::NewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void EditorDesigner::CreateMainMenuBar()
{
	if (ImGui::BeginMainMenuBar()) {
		m_mmb_menuBarSize = ImGui::GetWindowSize();
		if (ImGui::BeginMenu("File")) 
		{
			ImGui::MenuItem("Create", NULL, & m_projectSetup);
			if (ImGui::MenuItem("Open", "Ctrl+O --disabled")) 
			{
				EngineCoreEvents->FireEvent("OpenProject");
			}
			if (ImGui::MenuItem("Save", "Ctrl+S --disabled"))
			{
				EngineCoreEvents->FireEvent("SaveProject");
			}
			if (ImGui::MenuItem("Save as..")) 
			{
				EngineCoreEvents->FireEvent("SaveAsProject");
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("Explorer", "Ctrl+E", &m_explorerActive);
			ImGui::MenuItem("Properties", "Ctrl+P", &m_propertiesActive);
			ImGui::MenuItem("Instance Creator", NULL, &m_instanceCreatorActive);

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void EditorDesigner::CreateDockingPlace()
{
	ImGui::DockSpaceOverViewport(m_mainViewport, ImGuiDockNodeFlags_PassthruCentralNode);
}

void EditorDesigner::CreateExplorer(World* world, std::function<void(INSTANCE, INT&, INT&, ImGuiTreeNodeFlags&)> addFunction)
{
	if (!m_explorerActive) return;
	if (!m_isProjectLoaded) return;

	INT element = 0;
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
	
	ImGui::Begin("Explorer", nullptr, WindowFlags::EXPLORER_FLAGS);

	ImGui::Text("Selected: %d", m_pr_selected);

	if (m_pr_selected == element)
	{
		flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Selected;
	}

	if (ImGui::TreeNodeEx("World", flags))
	{
		if (ImGui::IsItemClicked() && ImGui::IsItemHovered())
		{
			m_pr_selected = element;
		}

		flags = ImGuiTreeNodeFlags_DefaultOpen;

		element++;

		for (auto instance : world->GetChildren())
		{
			flags = ImGuiTreeNodeFlags_DefaultOpen;

			if (m_pr_selected == element)
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
						m_pr_selected = element;
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
						m_pr_selected = element;
						EngineCoreEvents->FireEvent("EditorExplorerSelection", instance);
					}

					element++;

					addFunction(instance, element, m_pr_selected, flags);

					ImGui::TreePop();
				}
			}
		}

		ImGui::TreePop();
	}

	ImGui::End();
}

void EditorDesigner::CreateInstanceCreator(std::function<void(Vector3, Vector3, Vector3)> creationFunction)
{
	if (!m_instanceCreatorActive) return;
	if (!m_isProjectLoaded) return;

	ImGui::Begin("Instance", nullptr, WindowFlags::EXPLORER_FLAGS);

	ImGui::SeparatorText("Parameters:");
	ImGui::InputFloat3("Position", m_ic_posArray, "%.3f");
	ImGui::InputFloat3("Size", m_ic_sizeArray, "%.3f");
	ImGui::InputFloat3("Rotation", m_ic_rotArray, "%.3f");

	if (ImGui::Button("Add part"))
	{
		m_ic_position = Vector3{ m_ic_posArray[0], m_ic_posArray[1], m_ic_posArray[2] };
		m_ic_size = Vector3{ m_ic_sizeArray[0], m_ic_sizeArray[1], m_ic_sizeArray[2] };
		m_ic_rotation = Vector3{ m_ic_rotArray[0], m_ic_rotArray[1], m_ic_rotArray[2] };
		creationFunction(m_ic_position, m_ic_size, m_ic_rotation);
	};

	ImGui::End();
}

void EditorDesigner::CreateProperties(INSTANCE instance, ENG_PTR<World> world, INT item)
{
	if (!m_propertiesActive) return;
	if (!m_isProjectLoaded) return;

	static char name[50] = "";

	m_pr_nameBuffer = const_cast<char*>("");

	if (item == 0)
	{
		ImGui::Begin((m_pr_prefix + wstring_to_utf8(world->GetName()) + m_pr_id).c_str(), nullptr, WindowFlags::EXPLORER_FLAGS);
		m_pr_classStrName = "World";

		ImGui::InputText("ClassName", const_cast<char*>(m_pr_classStrName.c_str()), m_pr_classStrName.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::InputText("Name", const_cast<char*>(wstring_to_utf8(world->GetName()).c_str()), 32, ImGuiInputTextFlags_ReadOnly);
		ImGui::SeparatorText("");

		ImGui::InputText("Parent", const_cast<char*>("nil"), 3, ImGuiInputTextFlags_ReadOnly);

		ImGui::End();
		return;
	}

	if (instance)
	{
		switch (instance->GetInstanceClassName())
		{
		case WORLD:
		{
			World* obj = dynamic_cast<World*>(instance.get());
			if (obj)
			{
				std::string bufName = m_pr_prefix + wstring_to_utf8(instance->GetName()) + m_pr_id;

				ImGui::Begin(bufName.c_str(), nullptr, WindowFlags::EXPLORER_FLAGS);

				m_pr_classStrName = "World";

				ImGui::InputText("ClassName", const_cast<char*>(m_pr_classStrName.c_str()), m_pr_classStrName.length(), ImGuiInputTextFlags_ReadOnly);
				ImGui::InputText("Name", const_cast<char*>(wstring_to_utf8(obj->GetName()).c_str()), 32, ImGuiInputTextFlags_ReadOnly);
				ImGui::SeparatorText("");

				ImGui::InputText("Parent", const_cast<char*>("nil"), 3, ImGuiInputTextFlags_ReadOnly);
			}
			break;
		}
		case CAMERA:
		{
			Camera* obj = dynamic_cast<Camera*>(instance.get());
			if (obj)
			{
				std::string bufName = m_pr_prefix + wstring_to_utf8(instance->GetName()) + m_pr_id;

				ImGui::Begin(bufName.c_str(), nullptr, WindowFlags::EXPLORER_FLAGS);

				m_pr_classStrName = "Camera";

				ImGui::InputText("ClassName", const_cast<char*>(m_pr_classStrName.c_str()), m_pr_classStrName.length(), ImGuiInputTextFlags_ReadOnly);
				ImGui::InputText("Name", const_cast<char*>(wstring_to_utf8(obj->GetName()).c_str()), 32, ImGuiInputTextFlags_ReadOnly);
				ImGui::SeparatorText("");

				ImGui::InputText("Parent", const_cast<char*>(wstring_to_utf8(obj->GetParent()->GetName()).c_str()), 3, ImGuiInputTextFlags_ReadOnly);
			}
			break;
		}
		case SUNLIGHT:
		{
			Light* obj = dynamic_cast<Light*>(instance.get());
			if (obj)
			{
				std::string bufName = m_pr_prefix + wstring_to_utf8(instance->GetName()) + m_pr_id;

				ImGui::Begin(bufName.c_str(), nullptr, WindowFlags::EXPLORER_FLAGS);

				m_pr_classStrName = "Light";

				ImGui::InputText("ClassName", const_cast<char*>(m_pr_classStrName.c_str()), m_pr_classStrName.length(), ImGuiInputTextFlags_ReadOnly);
				ImGui::InputText("Name", const_cast<char*>(wstring_to_utf8(obj->GetName()).c_str()), 32, ImGuiInputTextFlags_ReadOnly);
				ImGui::SeparatorText("");

				ImGui::InputText("Parent", const_cast<char*>(wstring_to_utf8(obj->GetParent()->GetName()).c_str()), 3, ImGuiInputTextFlags_ReadOnly);
			}
			break;
		}
		case PART:
		{
			Part* obj = dynamic_cast<Part*>(instance.get());

			if (obj)
			{
				strcpy_s(name, const_cast<char*>(wstring_to_utf8(obj->GetName()).c_str()));

				std::string bufName = m_pr_prefix + wstring_to_utf8(instance->GetName()) + m_pr_id;

				ImGui::Begin(bufName.c_str(), nullptr, WindowFlags::EXPLORER_FLAGS);

				m_pr_classStrName = "Part";

				ImGui::InputText("ClassName", const_cast<char*>(m_pr_classStrName.c_str()), m_pr_classStrName.length(), ImGuiInputTextFlags_ReadOnly);
				if (ImGui::InputText("Name", name, 50, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					obj->SetName(utf8_to_wstring(name));
				}
				ImGui::SeparatorText("");

				ImGui::InputText("Parent", const_cast<char*>(wstring_to_utf8(obj->GetParent()->GetName()).c_str()), 3, ImGuiInputTextFlags_ReadOnly);
			}
			break;
		}
		default:
			ImGui::Begin("Properties: unknown###Properties", nullptr, WindowFlags::EXPLORER_FLAGS);
		}

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Properties: nil###Properties", nullptr, WindowFlags::EXPLORER_FLAGS);

		ImGui::End();
	}

}

void EditorDesigner::CreateProjectSetup(std::wstring& projectName, FileManager* fileMng, HWND hwnd)
{ 
	if (!m_projectSetup) return;

	ImGui::OpenPopup("Create Project");

	static char projectNameBuff[100] = "";
	std::string prjPath = wstring_to_utf8(fileMng->GetFileName());

	if (ImGui::BeginPopupModal("Create Project"))
	{
		ImGui::InputText("Project name", projectNameBuff, 100);

		ImGui::InputText("Project path", const_cast<char*>(prjPath.c_str()), prjPath.length(), ImGuiInputTextFlags_ReadOnly);

		ImGui::SameLine();

		if (ImGui::Button("..."))
		{
			fileMng->CallFileCreateScreen(hwnd);
		}

		if (ImGui::Button("Create"))
		{
			projectName = utf8_to_wstring(std::string(projectNameBuff));
			ImGui::CloseCurrentPopup();
			EngineCoreEvents->FireEvent("CreateNewProject");
			m_projectSetup = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
			m_projectSetup = false;
		}
		ImGui::EndPopup();
	}
}

void EditorDesigner::CreateCustomWindow(std::function<void()> window)
{
	window();
}

#endif
