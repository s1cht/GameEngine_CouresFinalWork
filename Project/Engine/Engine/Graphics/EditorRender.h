#pragma once
#include "pch.h"
#include "lib/ImGUI/imgui.h"
#include "lib/ImGUI/imgui_impl_dx9.h"

#pragma comment (lib, "d3d9.lib")

#ifdef SL_ENGINE_EDITOR

class EditorRender
{
public:
	EditorRender();
	~EditorRender();
public:
	bool Initialize(HWND);
	void Shutdown();

	void UpdateDevice(INT, INT);
	void ResetDevice();

	void EndScene();

	IDirect3DDevice9* GetDevice();

private:
	IDirect3DDevice9* m_device;

};

#endif
