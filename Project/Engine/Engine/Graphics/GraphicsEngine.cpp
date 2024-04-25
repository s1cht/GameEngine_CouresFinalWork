#include "pch.h"
#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine()
{
	m_Window = std::make_unique<Window>();
	m_Render = std::make_unique<Render>();
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
	if (!RenderSecond())
		return false;
}

bool GraphicsEngine::RenderSecond()
{
	m_Render->BeginScene(Color4{ 0.5f, 0.5f, 0.5f, 1 });

	m_Render->EndScene();

	return true;
}
