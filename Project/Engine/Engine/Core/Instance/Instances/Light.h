#pragma once
#include "pch.h"
#include "Engine/Core/Math/EngineMath.h"
#include "Engine/Core/Instance/Instance.h"

using namespace DirectX;

class Light : public Instance
{
public:
	Light();
	~Light() override;

	void SetDiffuseColor(Color4);
	void SetAmbientColor(Color4);
	void SetDirection(Vector3);
	void SetSpecularColor(Color4);
	void SetSpecularPower(FLOAT);

	XMFLOAT4 GetDiffuseColor();
	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetSpecularColor();
	XMFLOAT3 GetDirection();
	FLOAT GetSpecularPower();

	void SetParent(INSTANCE) override;
	void SetName(wstring) override;
	void SetDevices(ID3D11Device*, ID3D11DeviceContext*) override;
	void SetID(INT) override;

	void AddChild(INSTANCE) override;
	void DeleteChild(INT) override;

	INSTANCE GetParent() override;
	wstring GetName() override;
	std::vector<INSTANCE> GetChildren() override;
	ClassName GetInstanceClassName() override;
	INT GetID() override;
	INSTANCE GetChildByID(INT) override;

	INSTANCE operator[] (const wstring) override;
	INSTANCE operator[] (size_t) override;

	void Destroy() override;

private:
	ClassName m_ClassName = SUNLIGHT;

	INT m_ID;

	wstring m_Name;
	INSTANCE m_Parent;
	bool m_idInited = false;

	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_direction;
	XMFLOAT4 m_specularColor;
	FLOAT m_specularPower;

	std::vector<INSTANCE> m_Children;

};
