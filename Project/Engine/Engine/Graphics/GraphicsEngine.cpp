#include "pch.h"
#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine()
{
	m_Window = nullptr;
	m_Render = nullptr;
	m_Camera = nullptr;
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::Initialize()
{
	m_Window = std::make_unique<Window>(WINDOW_SIZE);
	if (!m_Window->Initialize(L"Window", m_hwnd))
	{
		MessageBox(m_hwnd, L"Window", L"Error", MB_OK);
		return false;
	}

	m_Render = std::make_unique<RenderClass>();
	if (!m_Render->Initialize(m_Window->GetSize(), VSYNC_ENABLED, m_hwnd, FULLSCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(m_hwnd, L"Render", L"Error", MB_OK);
		return false;
	}
	m_Camera = std::make_shared<Camera>();
	m_Camera->SetPosition(Vector3{ 0.f, 0.f, -3.f });

	m_Light = std::make_shared<Light>();
	m_Light->SetDiffuseColor(Color4{ 1.f, 1.f, 1.f, 1.f });
	m_Light->SetDirection(Vector3{ 0.f, 0.f, 1.f });

	return true;
}

void GraphicsEngine::Shutdown()
{
	if (m_Render)
		m_Render->Shutdown();
}

bool GraphicsEngine::Frame(Mesh*** meshes, Shader*** shaders, INT meshCount, INT shaderCount)
{
	static FLOAT rotation = 0.f;
	rotation -= math::ToRadians(1.f);

	if (!(rotation < 0.f))
		rotation += 360.f;

	if (!Render(rotation, meshes, shaders, meshCount, shaderCount))
		return false;

	return true;
}

HWND GraphicsEngine::GetHWND()
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

bool GraphicsEngine::Render(FLOAT rotation, Mesh*** meshes, Shader*** shaders, INT meshCount, INT shaderCount)
{
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix;
	Color4 color = { 0.f, 0.f, 0.f, 1.f };
	m_Render->BeginScene(color);

	m_Camera->Render();

	m_Render->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Render->GetProjectionMatrix(projectionMatrix);

	(*meshes)[meshCount - 1]->Render(m_Render->GetDeviceContext());

	worldMatrix = XMMatrixRotationRollPitchYaw(0.f, rotation, math::ToRadians(0.f));

	if (!(*shaders)[shaderCount - 1]->Render(
		m_Render->GetDeviceContext(), 
		(*meshes)[meshCount - 1]->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, 
		(*meshes)[meshCount - 1]->GetTexture(),
		m_Light->GetDirection(), m_Light->GetDiffuseColor()))
		return false;

	m_Render->EndScene();

	return true;
}
