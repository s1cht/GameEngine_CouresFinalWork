#include "pch.h"
#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine()
{
	m_Window = nullptr;
	m_Render = nullptr;
	m_ColorShader = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
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
	m_Camera->SetPosition(Vector3{ 0.f, 0.f, -5.f });

	m_Model = std::make_shared<ModelClass>();
	if (!m_Model->Initialize(m_Render->GetDevice()))
	{
		MessageBox(m_hwnd, L"Model", L"Error", MB_OK);
		return false;
	}

	m_ColorShader = std::make_unique<ColorShader>();
	if (!m_ColorShader->Initialize(m_Render->GetDevice(), m_hwnd))
	{
		MessageBox(m_hwnd, L"Shader", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsEngine::Shutdown()
{
	if (m_ColorShader)
		m_ColorShader->Shutdown();
	if (m_Model)
		m_Model->Shutdown();
	if (m_Render)
		m_Render->Shutdown();
}

bool GraphicsEngine::Frame()
{
	if (!Render())
		return false;
	return true;
}

bool GraphicsEngine::Render()
{
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix;
	Color4 color = { 0.27058825f, 0.48235294f, 0.94509804f, 1.f };
	m_Render->BeginScene(color);

	m_Camera->Render();

	m_Render->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Render->GetProjectionMatrix(projectionMatrix);

	m_Model->Render(m_Render->GetDeviceContext());

	if (!m_ColorShader->Render(m_Render->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))
		return false;

	m_Render->EndScene();

	return true;
}
