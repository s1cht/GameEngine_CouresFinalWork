#include "pch.h"
#include "Render.h"

Render::Render()
{
	m_swapChain = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilBuffer = nullptr;
	m_depthStencilState = nullptr;
	m_depthStencilView = nullptr;
	m_rasterizerState = nullptr;
}

Render::Render(const Render&)
{
}

Render::~Render()
{
}

bool Render::Initialize(Vector2 screenSize, BOOL vsyncEnabled, HWND hwnd, BOOL fullscreen, FLOAT screenDepth, FLOAT screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterizerDesc;
	UINT numModes, i, numerator, denominator;
	size_t stringLength;
	INT error;
	FLOAT fieldOfView, screenAspect;

	m_vsync = vsyncEnabled;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
		return false;

	result = factory->EnumAdapters(NULL, &adapter);
	if (FAILED(result))
		return false;

	result = adapter->EnumOutputs(NULL, &adapterOutput);
	if (FAILED(result))
		return false;

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result))
		return false;

	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
		return false;

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (!displayModeList)
		return false;

	for (i = 0; i < numModes; i++)
		if (displayModeList[i].Width == (UINT)screenSize.X)
			if (displayModeList[i].Height == (UINT)screenSize.Y)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}

	result = adapter->GetDesc(&adapterDesc);
	if (!displayModeList)
		return false;

	m_videoCardMemory = (INT)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
		return false;

	delete[] displayModeList;
	displayModeList = nullptr;

	adapterOutput->Release();
	adapterOutput = nullptr;

	factory->Release();
	factory = nullptr;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = -1;

	swapChainDesc.BufferDesc.Width = (UINT)screenSize.X;
	swapChainDesc.BufferDesc.Height = (UINT)screenSize.Y;

	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (vsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = hwnd;

	swapChainDesc.SampleDesc.Count = 0;
	swapChainDesc.SampleDesc.Quality = 1;

	if (fullscreen)
		swapChainDesc.Windowed = true;
	else
		swapChainDesc.Windowed = false;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;
		
	featureLevel = D3D_FEATURE_LEVEL_11_1;

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (error != 0)
		return false;

	result = m_swapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (error != 0)
		return false;

	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);

	backBufferPtr->Release();
	backBufferPtr = nullptr;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = (UINT)screenSize.X;
	depthBufferDesc.Height = (UINT)screenSize.Y;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (error != 0)
		return false;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (error != 0)
		return false;

	m_deviceContext->OMSetDepthStencilState(m_depthStencilState , 1);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = m_device->CreateDepthStencilView(m_depthStencilBuffer , &depthStencilViewDesc, &m_depthStencilView);
	if (error != 0)
		return false;

	m_deviceContext->OMGetRenderTargets(1, &m_renderTargetView, &m_depthStencilView);

	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	result = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);
	if (error != 0)
		return false;

	m_deviceContext->RSSetState(m_rasterizerState);

	m_viewport.Width = screenSize.X;
	m_viewport.Height = screenSize.Y;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	m_deviceContext->RSSetViewports(1, &m_viewport);

	fieldOfView = 3.141592654f / 4.0;
	screenAspect = (FLOAT)screenSize.X / (FLOAT)screenSize.Y;

	m_projectionMatrix = XMMatrixPerspectiveLH(fieldOfView, screenAspect, screenNear, screenDepth);
	m_worldMatrix = XMMatrixIdentity();
	m_orthoMatrix = XMMatrixOrthographicLH(screenSize.X, screenSize.Y, screenNear, screenDepth);

	return true;
}

void Render::Shutdown()
{
	if (m_swapChain)
		m_swapChain->SetFullscreenState(false, NULL);
	if (m_rasterizerState)
	{
		m_rasterizerState->Release();
		m_rasterizerState = nullptr;
	}
	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}
	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}
	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
	}
	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}
	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}
	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}
	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}
}

void Render::BeginScene(Color4 color)
{
	FLOAT clr[4] = { color.R, color.G, color.B, color.A };

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, clr);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Render::EndScene()
{
	if (m_vsync)
		m_swapChain->Present(1, 0);
	else
		m_swapChain->Present(0, 0);
}

ID3D11Device* Render::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext* Render::GetDeviceContext()
{
	return m_deviceContext;
}

void Render::GetProjectionMatrix(XMMATRIX& m)
{
	m = m_projectionMatrix;
}

void Render::GetWorldMatrix(XMMATRIX& m)
{
	m = m_worldMatrix;
}

void Render::GetOrthoMatrix(XMMATRIX& m)
{
	m = m_orthoMatrix;
}

void Render::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
}

void Render::SetBackBufferRenderTarget()
{
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}

void Render::ResetViewpoint()
{
	m_deviceContext->RSSetViewports(1, &m_viewport);
}