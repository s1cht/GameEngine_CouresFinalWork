#pragma once

#include "pch.h"

using namespace DirectX;

class ModelClass
{
private:
	struct VertexType 
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	ModelClass();
	~ModelClass();

	bool Initialize(ID3D11Device*);
	void Shutdown();

	void Render(ID3D11DeviceContext*);

	INT GetIndexCount();
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	INT m_vertexCount, m_indexCount;
};

