#pragma once

#include "pch.h"
#include "Instance.h"
#include "Texture.h"
#include "Engine/Core/Math/EngineMath.h"
#include "ResourceManager.h"

using namespace std;
using namespace DirectX;

/*
class Part : Instance
{
public:
	virtual ~Part() = default;

	virtual void Destroy() override;

	virtual bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	virtual void Shutdown();

	virtual void Render(ID3D11DeviceContext*);

	virtual void SetPosition(Vector3);
	virtual void SetRotation(Vector3);
	virtual void SetSize(Vector3);

	virtual Vector3 GetPosition();
	virtual Vector3 GetRotation();
	virtual Vector3 GetSize();

	virtual INT GetIndexCount();
	virtual ID3D11ShaderResourceView* GetTexture();

protected:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

	void ReleaseModel();

protected:
	wstring Name;
	Instance* Parent;

	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_size;

	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	INT m_vertexCount, m_indexCount;

	TextureClass* m_Texture;
	ModelType* m_model;

protected:
	Event<void> OnDestroyed;
	Event<Vector3> Moved;
	Event<Vector3> Rotated;
	Event<Vector3> Resized;

};



*/