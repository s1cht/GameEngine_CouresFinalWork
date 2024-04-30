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
	m_Camera->SetPosition(Vector3{ -5.f, 0.f, 0.f });

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
	Color4 color = { 0.27058825f, 0.48235294f, 0.94509804f, 0.f };
	m_Render->BeginScene(color);

	m_Render->EndScene();

	return true;
}
