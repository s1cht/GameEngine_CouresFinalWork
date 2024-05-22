#pragma once
#include "pch.h"
#include "Engine/Core/Math/EngineMath.h"

class RenderToTexture
{
public:
	RenderToTexture();
	~RenderToTexture();
public:
	bool Initialize(ID3D11Device*, Vector2);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void ClearRenderTarget(ID3D11DeviceContext*, Color4);

	bool ResizeTexture(ID3D11Device*, Vector2);

	Vector2 GetSize();
	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_resourceView;

	Vector2 m_size;

};

