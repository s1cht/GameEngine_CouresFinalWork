#pragma once
#include "pch.h"
#include "Engine/Core/Math/EngineMath.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

class RenderClass
{
public:
	RenderClass();
	~RenderClass();
public:
	bool Initialize(Vector2, BOOL, HWND, BOOL, FLOAT, FLOAT);
	void Shutdown();

	void BeginScene(Color4);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11DepthStencilView* GetDepthStencilView();

	ID3D11ShaderResourceView* GetResourceView();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

	bool ResizeDirectX(Vector2);
	void ResetViewpoint();

private:
	BOOL m_vsync;
	INT m_videoCardMemory;
	char m_videoCardDescription[128];
	Vector2 m_frameSize;

	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterizerState;

	ID3D11Texture2D* m_renderTargetImage;
	ID3D11ShaderResourceView* m_resourceView;
	ID3D11RenderTargetView* m_renderTextureTargetView;

	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;

	D3D11_VIEWPORT m_viewport;

};

