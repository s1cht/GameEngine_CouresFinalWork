#include "pch.h"
#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine()
{
	m_Window = std::make_unique<Window>(WINDOW_SIZE);
	m_Render = std::make_unique<RenderClass>();
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::Initialize()
{
	if (!m_Window->Initialize(L"Window", m_hwnd))
		return false;
	if (!m_Render->Initialize(m_Window->GetSize(), VSYNC_ENABLED, m_hwnd, FULLSCREEN, SCREEN_DEPTH, SCREEN_NEAR))
		return false;

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
