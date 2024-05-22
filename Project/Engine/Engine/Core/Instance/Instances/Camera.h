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
	void SetVelocity(Vector3);

	Vector3 GetPosition();
	Vector3 GetRotation();
	Vector3 GetUpVector();
	Vector3 GetForwardpVector();
	Vector3 GetRightVector();

	void Render(FLOAT, FLOAT);
	void GetViewMatrix(XMMATRIX&);

	void SetParent(INSTANCE) override;
	void SetName(wstring) override;
	void SetDevices(ID3D11Device*, ID3D11DeviceContext*) override;
	void SetID(INT) override;

	void AddChild(INSTANCE) override;
	void DeleteChild(INT) override;

	INSTANCE GetParent() override;
	wstring GetName() override;
	ClassName GetInstanceClassName() override;
	std::vector<INSTANCE> GetChildren() override;
	INT GetID() override;
	INSTANCE GetChildByID(INT) override;

	INSTANCE operator[] (const wstring) override;
	INSTANCE operator[] (size_t) override;

	void Destroy() override;

private:
	ClassName m_ClassName = CAMERA;

	INT m_ID;

	wstring m_Name;
	INSTANCE m_Parent;
	bool m_idInited = false;

	std::vector<INSTANCE> m_Children;

	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_velocity;
	XMMATRIX m_viewMatrix;

	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.f, 0.f, 0.f, 0.f);;

	XMVECTOR m_upVector;
	XMVECTOR m_forwardVector;
	XMVECTOR m_rightVector;
	XMVECTOR m_positionVector;
	XMVECTOR m_lookAtVector;

};