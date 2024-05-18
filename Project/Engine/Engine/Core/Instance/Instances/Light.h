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

	void SetParent(Instance*) override;
	void SetName(wstring) override;
	void SetDevices(ID3D11Device*, ID3D11DeviceContext*) override;

	void AddChild(Instance*) override;
	void DeleteChild(std::wstring) override;

	Instance* GetParent() override;
	wstring GetName() override;
	std::vector<Instance*> GetChildren() override;

	Instance* operator[] (const wstring) override;
	Instance* operator[] (size_t) override;

	void Destroy() override;

private:
	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_direction;
	XMFLOAT4 m_specularColor;
	FLOAT m_specularPower;

	using ClassName = Instance;

	wstring m_Name;
	Instance* m_Parent;

	std::vector<Instance*> m_Children;

};
