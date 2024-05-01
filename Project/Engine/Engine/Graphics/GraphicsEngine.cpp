#include "pch.h"
#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine()
{
	m_Window = nullptr;
	m_Render = nullptr;
	m_LightShader = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::Initialize()
{
	char textureFileName[128];
	char modelFileName[128];

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
	
	strcpy_s(textureFileName, "../Engine/Assets/Textures/stone01.tga");
	strcpy_s(modelFileName, "../Engine/Assets/Models/cube.txt");

	m_Model = std::make_shared<ModelClass>();
	if (!m_Model->Initialize(m_Render->GetDevice(), m_Render->GetDeviceContext(), textureFileName, modelFileName))
	{
		MessageBox(m_hwnd, L"Model", L"Error", MB_OK);
		return false;
	}

	m_Light = std::make_shared<Light>();
	m_Light->SetDiffuseColor(Color4{ 1.f, 1.f, 1.f, 1.f });
	m_Light->SetDirection(Vector3{ 0.f, 0.f, 1.f });

	m_LightShader = std::make_unique<LightShader>();
	if (!m_LightShader->Initialize(m_Render->GetDevice(), m_hwnd))
	{
		MessageBox(m_hwnd, L"Shader", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsEngine::Shutdown()
{
	if (m_LightShader)
		m_LightShader->Shutdown();
	if (m_Model)
		m_Model->Shutdown();
	if (m_Render)
		m_Render->Shutdown();
}

bool GraphicsEngine::Frame()
{
	static FLOAT rotation = 0.f;
	rotation -= math::ToRadians(1.f);

	if (!(rotation < 0.f))
		rotation += 360.f;

	if (!Render(rotation))
		return false;

	return true;
}

bool GraphicsEngine::Render(FLOAT rotation)
{
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix;
	Color4 color = { 0.f, 0.f, 0.f, 1.f };
	m_Render->BeginScene(color);

	m_Camera->Render();

	m_Render->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Render->GetProjectionMatrix(projectionMatrix);

	m_Model->Render(m_Render->GetDeviceContext());

	worldMatrix = XMMatrixRotationRollPitchYaw(0.f, rotation, math::ToRadians(0.f));

	if (!m_LightShader->Render(
		m_Render->GetDeviceContext(), 
		m_Model->GetIndexCount(), 
		worldMatrix, viewMatrix, projectionMatrix, 
		m_Model->GetTexture(), 
		m_Light->GetDirection(), m_Light->GetDiffuseColor()))
		return false;

	m_Render->EndScene();

	return true;
}
