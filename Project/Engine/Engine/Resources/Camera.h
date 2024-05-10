#pragma once
#include "pch.h"
#include "Engine/Core/Math/EngineMath.h"
#include "Engine/Core/Instance/Instance.h"

using namespace DirectX;

class Camera : public Instance
{
public:
	Camera();
	~Camera() override;

	void SetPosition(Vector3);
	void SetRotation(Vector3);

	Vector3 GetPosition();
	Vector3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);

	void SetParent(Instance*) override;
	void SetName(wstring) override;

	void AddChild(Instance*) override;
	void DeleteChild(std::wstring) override;

	Instance*& GetParent() override;
	wstring GetName() override;
	std::vector<Instance*>& GetChildren() override;

	Instance* operator[] (const wstring) override;
	Instance* operator[] (size_t) override;

	void Destroy() override;

private:
	using ClassName = Instance;

	wstring m_Name;
	Instance* m_Parent;

	std::vector<Instance*> m_Children;

	Vector3 m_position;
	Vector3 m_rotation;
	XMMATRIX m_viewMatrix;

};