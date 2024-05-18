#include "pch.h"
#include "EditorRender.h"


#ifdef SL_ENGINE_EDITOR

static D3DPRESENT_PARAMETERS deviceParams = {};

EditorRender::EditorRender()
{
	m_device = nullptr;
}

EditorRender::~EditorRender()
{
}

bool EditorRender::Initialize(HWND hwnd)
{
	HRESULT result;
	IDirect3D9* factory;

	factory = Direct3DCreate9(D3D_SDK_VERSION);
	if (!factory)
		return false;

	ZeroMemory(&deviceParams, sizeof(deviceParams));

	deviceParams.Windowed = TRUE;
	deviceParams.SwapEffect = D3DSWAPEFFECT_COPY;
	deviceParams.BackBufferFormat = D3DFMT_UNKNOWN;
	deviceParams.EnableAutoDepthStencil = TRUE;
	deviceParams.AutoDepthStencilFormat = D3DFMT_D16;
	deviceParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	result = factory->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&deviceParams, &m_device);
	if (FAILED(result))
		return false;

	factory->Release();
	factory = nullptr;

	return true;
}

void EditorRender::Shutdown()
{
	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}
}

void EditorRender::UpdateDevice(INT w, INT h)
{
	if (w != 0 && h != 0)
	{
		deviceParams.BackBufferWidth = w;
		deviceParams.BackBufferHeight = h;
		ResetDevice();
	}
}

void EditorRender::ResetDevice()
{
	HRESULT result;
	ImGui_ImplDX9_InvalidateDeviceObjects();
	result = m_device->Reset(&deviceParams);
	//if (result == D3DERR_INVALIDCALL)
	//	IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

void EditorRender::EndScene()
{
	HRESULT result;
	D3DCOLOR color = D3DCOLOR_RGBA(5, 5, 5, 255);
	ImGui::EndFrame();
	m_device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	m_device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);

	if (m_device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		m_device->EndScene();
	}
	result = m_device->Present(nullptr, nullptr, nullptr, nullptr);

	if (result == D3DERR_DEVICELOST && m_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}

IDirect3DDevice9* EditorRender::GetDevice()
{
	return m_device;
}

#endif
