#pragma once

#include "pch.h"
#include "TextureClass.h"

using namespace DirectX;

class ModelClass
{
private:
	struct VertexType 
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	ModelClass();
	~ModelClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	void Render(ID3D11DeviceContext*);

	INT GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	INT m_vertexCount, m_indexCount;

	TextureClass* m_Texture;
};

