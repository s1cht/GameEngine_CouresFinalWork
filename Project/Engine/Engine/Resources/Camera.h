#pragma once
#include "pch.h"
#include "Engine/Core/Math/EngineMath.h"

using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(Vector3);
	void SetRotation(Vector3);

	Vector3 GetPosition();
	Vector3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);
private:
	Vector3 m_position;
	Vector3 m_rotation;
	XMMATRIX m_viewMatrix;

};

