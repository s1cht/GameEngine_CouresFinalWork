#include "pch.h"
#include "UIRender.h"


UIRender::UIRender()
{
	m_RenderTarget = nullptr;
}

UIRender::~UIRender()
{
}

bool UIRender::Initialize(HWND hwnd)
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
	GetClientRect(m_hwnd, &rc);

	renderTargetProperties.type =				D2D1_RENDER_TARGET_TYPE_DEFAULT;
	renderTargetProperties.pixelFormat =		D2D1_PIXEL_FORMAT(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
	renderTargetProperties.dpiX =				0;
	renderTargetProperties.dpiY =				0;
	renderTargetProperties.usage =				D2D1_RENDER_TARGET_USAGE_NONE;
	renderTargetProperties.minLevel =			D2D1_FEATURE_LEVEL_10;

	hwndRenderTargetProperties.hwnd =			hwnd;
	hwndRenderTargetProperties.pixelSize =		D2D1_SIZE_U(rc.right - rc.left, rc.bottom - rc.top);
	hwndRenderTargetProperties.presentOptions = D2D1_PRESENT_OPTIONS_IMMEDIATELY;

	result = factory->CreateHwndRenderTarget(renderTargetProperties, hwndRenderTargetProperties, &m_RenderTarget);
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

bool UIRender::Frame()
{
	if (!Render())
		return false;

	return true;
}

bool UIRender::Render()
{
	HRESULT result;
	ID2D1SolidColorBrush* brush;
	RECT rc;

	m_RenderTarget->CreateSolidColorBrush(D2D1_COLOR_F(D2D1::ColorF::White), &brush);

	GetClientRect(m_hwnd, &rc);

	m_RenderTarget->BeginDraw();
	
	m_RenderTarget->DrawRectangle(
		D2D1::RectF(
			rc.left + 100.0f,
			rc.top + 100.0f,
			rc.right - 100.0f,
			rc.bottom - 100.0f),
		brush);

	result = m_RenderTarget->EndDraw();
	if (FAILED(result))
		return false;

	return true;
}