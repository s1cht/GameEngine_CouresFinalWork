#pragma once
#include "pch.h"
#include "Engine/Core/Math/EngineMath.h"

using namespace DirectX;

class Light
{
public:
	Light();
	~Light();

	void SetDiffuseColor(Color4);
	void SetDirection(Vector3);

	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();

private:
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_direction;

};
