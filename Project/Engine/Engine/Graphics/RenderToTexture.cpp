#include "pch.h"
#include "RenderToTexture.h"

RenderToTexture::RenderToTexture()
{
	m_renderTargetTexture = nullptr;
	m_renderTargetView = nullptr;
	m_resourceView = nullptr;
	m_size = Vector2::Zero();
}

RenderToTexture::~RenderToTexture()
{
}

bool RenderToTexture::Initialize(ID3D11Device* device, Vector2 size)
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC renderTargetTextureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

	ZeroMemory(&renderTargetTextureDesc, sizeof(renderTargetTextureDesc));
	renderTargetTextureDesc.Width = (UINT)size.X;
	renderTargetTextureDesc.Height = (UINT)size.Y;
	renderTargetTextureDesc.MipLevels = 1;
	renderTargetTextureDesc.ArraySize = 1;
	renderTargetTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTargetTextureDesc.SampleDesc.Count = 1;
	renderTargetTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	renderTargetTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	renderTargetTextureDesc.CPUAccessFlags = 0;
	renderTargetTextureDesc.MiscFlags = 0;
	renderTargetTextureDesc.SampleDesc.Count = 1;
	renderTargetTextureDesc.SampleDesc.Quality = 0;
	
	renderTargetViewDesc.Format = renderTargetTextureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;


	result = device->CreateTexture2D(&renderTargetTextureDesc, NULL, &m_renderTargetTexture);
	if (FAILED(result))
		return false;

	shaderResourceViewDesc.Format = renderTargetTextureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	result = device->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_resourceView);
	if (FAILED(result))
		return false;

	result = device->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
	if (FAILED(result))
		return false;

	return true;
}

void RenderToTexture::Shutdown()
{
	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}
	if (m_resourceView)
	{
		m_resourceView->Release();
		m_resourceView = nullptr;
	}
	if (m_renderTargetTexture)
	{
		m_renderTargetTexture->Release();
		m_renderTargetTexture = nullptr;
	}
}

void RenderToTexture::SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView)
{
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext->OMSetRenderTargets(1, &m_renderTargetView, depthStencilView);
}

void RenderToTexture::ClearRenderTarget(ID3D11DeviceContext* deviceContext, Color4 color)
{
	float clr[4];

	clr[0] = color.R;
	clr[1] = color.G;
	clr[2] = color.B;
	clr[3] = color.A;

	deviceContext->ClearRenderTargetView(m_renderTargetView, clr);
}

bool RenderToTexture::ResizeTexture(ID3D11Device* device, Vector2 size)
{
	if (size == Vector2::Zero())
		return true;
	Shutdown();
	if (!Initialize(device, size))
		return false;
	
	return true;
}

Vector2 RenderToTexture::GetSize()
{
	return m_size;
}

ID3D11ShaderResourceView* RenderToTexture::GetShaderResourceView()
{
	return m_resourceView;
}
