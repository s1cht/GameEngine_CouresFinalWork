#include "pch.h"
#include "UIRender.h"


UIRender::UIRender()
{
	m_RenderTarget = nullptr;
}

UIRender::~UIRender()
{
}

bool UIRender::Initialize(HWND hwnd, IDXGISurface* dxgiSurface)
{
	HRESULT result;
	ID2D1Factory* factory;
	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRenderTargetProperties;

	factory = nullptr;
	m_hwnd = hwnd;

	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory), (void**)&factory);
	if (FAILED(result))
		return false;

	RECT rc;
	FLOAT dpi = GetDpiForWindow(m_hwnd);
	GetWindowRect(m_hwnd, &rc);

	renderTargetProperties.type =				D2D1_RENDER_TARGET_TYPE_DEFAULT;
	renderTargetProperties.pixelFormat =		D2D1_PIXEL_FORMAT(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
	renderTargetProperties.dpiX =				static_cast<int>(ceil((rc.right - rc.left) * dpi / 96.f));
	renderTargetProperties.dpiY =				static_cast<int>(ceil((rc.bottom - rc.top) * dpi / 96.f));
	renderTargetProperties.usage =				D2D1_RENDER_TARGET_USAGE_NONE;
	renderTargetProperties.minLevel =			D2D1_FEATURE_LEVEL_10;

	result = factory->CreateDxgiSurfaceRenderTarget(dxgiSurface, &renderTargetProperties, &m_RenderTarget);
	if (FAILED(result))
		return false;

	factory->Release();
	factory = nullptr;

	return true;
}

void UIRender::Shutdown()
{
	if (m_RenderTarget)
	{
		m_RenderTarget->Release();
		m_RenderTarget = nullptr;
	}
}

ID2D1RenderTarget* UIRender::GetRenderTarget()
{
	return m_RenderTarget;
}

void UIRender::BeginScene()
{
	m_RenderTarget->BeginDraw();
}

bool UIRender::EndScene()
{
	HRESULT result = m_RenderTarget->EndDraw();
	if (FAILED(result))
		return false;
	return true;
}
