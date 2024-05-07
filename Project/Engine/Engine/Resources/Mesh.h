#pragma once

#include "pch.h"
#include "Texture.h"

using namespace DirectX;

class Mesh
{
private:
	struct VertexType 
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};
	struct ModelType
	{
		FLOAT x, y, z;
		FLOAT tu, tv;
		FLOAT nx, ny, nz;
	};

public:
	Mesh();
	~Mesh();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, INT, INT, ModelType*);
	void Shutdown();

	void Render(ID3D11DeviceContext*);

	void SetName(std::string);

	INT GetIndexCount();
	std::string GetName();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	void ReleaseTexture();

	void ReleaseModel();

	static bool ReadObjectFile(const char* fileName, INT& vertexCount, INT& indexCount, ModelType*& object);

private:
	std::string m_name;

	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	INT m_vertexCount, m_indexCount;

	ModelType* m_model;

private:
	friend class ResourceManager;
};

