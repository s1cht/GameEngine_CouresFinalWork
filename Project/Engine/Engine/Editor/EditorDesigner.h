#pragma once
#include "pch.h"
#include "EditorFlags.h"
#include "Engine/Core/Math/EngineMath.h"
#include "Engine/Core/Events/CoreEvents.h"
#include "Engine/Core/Instance/Instance.h"
#include "Engine/Core/Game/World.h"
#include "Engine/Core/Instance/Instances/Camera.h"
#include "Engine/Core/Instance/Instances/Light.h"
#include "Engine/Core/Instance/Instances/Part.h"
#include "Engine/Resources/FileManager.h"

std::string wstring_to_utf8(const std::wstring&);
std::wstring utf8_to_wstring(const std::string&);


#ifdef SL_ENGINE_EDITOR

class EditorDesigner
{
public:
	EditorDesigner();
	~EditorDesigner();

	bool Initialize(HWND, ID3D11Device*, ID3D11DeviceContext*);
	void Shutdown();

	void SetProjectLoaded(bool);

	INT GetSelectedExplorerItem();

	void NewFrame();

	void CreateMainMenuBar();
	void CreateDockingPlace();

	void CreateExplorer(World*, std::function<void(INSTANCE, INT&, INT&, ImGuiTreeNodeFlags&)>);
	void CreateInstanceCreator(std::function<void(Vector3, Vector3, Vector3)>);
	void CreateProperties(INSTANCE, ENG_PTR<World>, INT);
	void CreateProjectSetup(std::wstring&, FileManager*, HWND);

	void CreateCustomWindow(std::function<void()>);

	std::function<void(ID3D11ShaderResourceView*)> RenderWindow;

private: // General
	ImGuiIO* m_io;
	ImGuiViewport* m_mainViewport;
	bool m_isProjectLoaded = false;

private: // Main menu bar
	ImVec2 m_mmb_menuBarSize;

private: // Render
	ImVec2 m_r_imageSize;
	ImVec2 m_r_imagePosition;
	ImVec2 m_r_windowRegionSize;

private: // Explorer
	bool m_explorerActive;

private: // Properties
	bool m_propertiesActive;

	INT m_pr_selected = 0;

	std::string m_pr_id = "###Properties";
	std::string m_pr_prefix = "Properties: ";

	std::string m_pr_name;
	std::string m_pr_classStrName;
	ClassName m_pr_className;
	char* m_pr_nameBuffer;


	Vector3 m_pr_position;
	Vector3 m_pr_size;
	Vector3 m_pr_rotation;
	FLOAT m_pr_posArray[3];
	FLOAT m_pr_sizeArray[3];
	FLOAT m_pr_rotArray[3];

private: // Explorer
	bool m_projectSetup;


private: // Instance creator
	bool m_instanceCreatorActive;
	Vector3 m_ic_position;
	Vector3 m_ic_size;
	Vector3 m_ic_rotation;
	FLOAT m_ic_posArray[3];
	FLOAT m_ic_sizeArray[3];
	FLOAT m_ic_rotArray[3];

};

#endif