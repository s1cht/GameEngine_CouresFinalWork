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

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, Texture*, INT, INT, ModelType*);
	void Shutdown();

	void Render(ID3D11DeviceContext*);

	void SetName(std::string);
	void SetTexture(Texture*);

	INT GetIndexCount();
	std::string GetName();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	void LoadTexture(Texture*);
	void ReleaseTexture();

	void ReleaseModel();

	static bool ReadObjectFile(const char* fileName, INT& vertexCount, INT& indexCount, ModelType*& object);

private:
	std::string m_name;

	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	INT m_vertexCount, m_indexCount;

	Texture* m_Texture;
	ModelType* m_model;

private:
	friend class ResourceManager;
};

