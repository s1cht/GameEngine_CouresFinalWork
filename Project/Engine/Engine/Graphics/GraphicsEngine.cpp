#include "pch.h"
#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine()
{
	m_Window = nullptr;
	m_Render = nullptr;
	m_hwnd = nullptr;
	m_newSize = Vector2::Zero();
	m_windowResized = false;
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::Initialize()
{
	m_Window = std::make_unique<Window>(WINDOW_SIZE);

	if (!m_Window->Initialize(WINDOW_NAME, m_hwnd))
	{
		MessageBox(m_hwnd, L"Window", L"Error", MB_OK);
		return false;
	}

	m_Render = std::make_unique<RenderClass>();
	if (!m_Render->Initialize(
		WINDOW_SIZE,
		VSYNC_ENABLED, 
		m_hwnd,
		FULLSCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(m_hwnd, L"Render", L"Error", MB_OK);
		return false;
	}

	m_RenderToTexture = std::make_unique<RenderToTexture>();
	if (!m_RenderToTexture->Initialize(m_Render->GetDevice(), WINDOW_SIZE))
	{
		MessageBox(m_hwnd, L"Render to texture", L"Error", MB_OK);
		return false;
	}

	EngineCoreEvents->AddListener<Vector2>([&](EVENT_RET_TYPE<Vector2> size) { m_windowResized = true; m_newSize.X = size->X; m_newSize.Y = size->Y; }, "WindowUpdated");

	return true;
}

void GraphicsEngine::Shutdown()
{
	if (m_Render)
		m_Render->Shutdown();
	if (m_RenderToTexture)
		m_RenderToTexture->Shutdown();
}

bool GraphicsEngine::Frame(ENG_PTR<World> world, Shader**& shaders, INT meshCount, INT shaderCount, FLOAT pitch, FLOAT yaw
#ifdef SL_ENGINE_EDITOR
	, std::function<void(ID3D11ShaderResourceView*)>& editorOutput, bool isLoaded
#endif
)
{

	if (!Render(world, shaders, meshCount, shaderCount, pitch, yaw
#ifdef SL_ENGINE_EDITOR
		,editorOutput, isLoaded
#endif
	))
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

ID3D11ShaderResourceView* GraphicsEngine::GetRenderResource()
{
	return m_RenderToTexture->GetShaderResourceView();
}

bool GraphicsEngine::Render(ENG_PTR<World> world, Shader**& shaders, INT meshCount, INT shaderCount, FLOAT pitch, FLOAT yaw
#ifdef SL_ENGINE_EDITOR
	,std::function<void(ID3D11ShaderResourceView*)>& editorOutput, bool isLoaded
#endif
)
{
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix, rotateMatrix, translateMatrix, scaleMatrix, srMatrix;
	Color4 color = { 0.14f, 0.14f, 0.14f, 1.f };
	Color4 colorRender = { 0.f, 0.f, 0.f, 1.f };

	if (m_windowResized)
	{
		Vector2 size = m_newSize;
		if (m_Render->ResizeDirectX(size) && m_RenderToTexture->ResizeTexture(m_Render->GetDevice(), size))
			m_windowResized = false;
		else
		{
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			return false;
		}
	}

#ifdef SL_ENGINE_EDITOR
	if (!isLoaded)
	{
		m_Render->BeginScene(color);

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		m_Render->EndScene();

		return true;
	}
#endif
	Camera* camera =	dynamic_cast<Camera*>(world->operator[](L"Camera").get());
	Light* light =		dynamic_cast<Light*>(world->operator[](L"SunLight").get());

	static std::function<bool(Part*)> renderPart = [&](Part* part) -> bool
		{
			for (auto& object : part->GetChildren())
			{
				Part* part = dynamic_cast<Part*>(object.get());
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
			return true;
		};

	m_Render->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	m_Render->GetProjectionMatrix(projectionMatrix);

#ifndef SL_ENGINE_EDITOR
	m_Render->BeginScene(color);
#else
	m_RenderToTexture->ClearRenderTarget(m_Render->GetDeviceContext(), colorRender);
	m_RenderToTexture->SetRenderTarget(m_Render->GetDeviceContext(), m_Render->GetDepthStencilView());
#endif

	camera->Render(pitch, yaw);

	for (auto& object : world->GetChildren())
	{
		Part* part = dynamic_cast<Part*>(object.get());

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
			if (!renderPart(part))
				return false;
		}
	}

#ifdef SL_ENGINE_EDITOR
	m_Render->BeginScene(color);

	editorOutput(m_RenderToTexture->GetShaderResourceView());

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
	m_Render->EndScene();

	return true;
}
