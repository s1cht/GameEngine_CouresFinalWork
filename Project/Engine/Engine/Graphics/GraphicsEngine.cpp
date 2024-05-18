#include "pch.h"
#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine()
{
	m_Window = nullptr;
	m_Render = nullptr;
	m_hwnd = nullptr;
#ifdef SL_ENGINE_EDITOR
	m_hwndRender = nullptr;
#endif
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::Initialize()
{
	m_Window = std::make_unique<Window>(WINDOW_SIZE);

#ifndef SL_ENGINE_EDITOR
	if (!m_Window->Initialize(WINDOW_NAME, m_hwnd))
	{
		MessageBox(m_hwnd, L"Window", L"Error", MB_OK);
		return false;
	}
#else
	if (!m_Window->Initialize(WINDOW_NAME, m_hwnd, m_hwndRender))
	{
		MessageBox(m_hwnd, L"Window", L"Error", MB_OK);
		return false;
	}
#endif

	m_Render = std::make_unique<RenderClass>();
	if (!m_Render->Initialize(
#ifndef SL_ENGINE_EDITOR
		m_Window->GetSize(),
#else
		m_Window->GetRenderSize(),
#endif
		VSYNC_ENABLED, 
		m_hwnd,
		FULLSCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(m_hwnd, L"Render", L"Error", MB_OK);
		return false;
	}

#ifdef SL_ENGINE_EDITOR
	m_EditorRender = std::make_unique<EditorRender>();
	if (!m_EditorRender->Initialize(m_hwnd))
	{
		MessageBox(m_hwnd, L"Editor render", L"Error", MB_OK);
		return false;
	}
#endif

	return true;
}

#ifdef SL_ENGINE_EDITOR

bool GraphicsEngine::UpdateEditor(INT width, INT heigth)
{
	m_EditorRender->UpdateDevice(width, heigth);
	return true;
}

bool GraphicsEngine::EditorFrame()
{
	m_EditorRender->EndScene();
	return true;
}

IDirect3DDevice9* GraphicsEngine::GetEditorDevice()
{
	return m_EditorRender->GetDevice();
}
#endif

void GraphicsEngine::Shutdown()
{
	if (m_Render)
		m_Render->Shutdown();
}

bool GraphicsEngine::Frame(World*& world, Shader**& shaders, INT meshCount, INT shaderCount)
{
	if (!Render(world, shaders, meshCount, shaderCount))
		return false;

	return true;
}

HWND GraphicsEngine::GetHWND() const
{
	return m_hwnd;
}

ID3D11Device* GraphicsEngine::GetDevice()
{
	return m_Render->GetDevice();
}

ID3D11DeviceContext* GraphicsEngine::GetDeviceContext()
{
	return m_Render->GetDeviceContext();
}

bool GraphicsEngine::Render(World*& world, Shader**& shaders, INT meshCount, INT shaderCount)
{
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix, rotateMatrix, translateMatrix, scaleMatrix, srMatrix;
	Color4 color = { 0.f, 0.f, 0.f, 1.f };

	Camera* camera =	dynamic_cast<Camera*>(world->operator[](L"Camera"));
	Light* light =		dynamic_cast<Light*>(world->operator[](L"SunLight"));

	m_Render->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	m_Render->GetProjectionMatrix(projectionMatrix);

	m_Render->BeginScene(color);

	camera->Render();

	for (auto& object : world->GetChildren())
	{
		Part* part = dynamic_cast<Part*>(object);

	if (part)
		{
			rotateMatrix = XMMatrixRotationRollPitchYaw(part->GetRotation().X, part->GetRotation().Y, part->GetRotation().Z);
			translateMatrix = XMMatrixTranslation(part->GetPosition().X, part->GetPosition().Y, part->GetPosition().Z);
			scaleMatrix = XMMatrixScaling(part->GetSize().X, part->GetSize().Y, part->GetSize().Z);

			srMatrix = XMMatrixMultiply(scaleMatrix, rotateMatrix);
			worldMatrix = XMMatrixMultiply(srMatrix, translateMatrix);

			part->GetMesh()->Render(m_Render->GetDeviceContext());

			if (!shaders[shaderCount - 1]->Render(
				m_Render->GetDeviceContext(),
				worldMatrix, viewMatrix, projectionMatrix,
				part,
				light, XMFLOAT3{ camera->GetPosition().X, camera->GetPosition().Y, camera->GetPosition().Z }))
				return false;
		}
	}

	m_Render->EndScene();

	return true;
}
