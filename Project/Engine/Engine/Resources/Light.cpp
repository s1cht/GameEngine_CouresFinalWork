#include "pch.h"
#include "Light.h"

Light::Light()
{
}

Light::~Light()
{
}

void Light::SetDiffuseColor(Color4 color)
{
    m_diffuseColor = XMFLOAT4(color.R, color.G, color.B, color.A);
}

void Light::SetDirection(Vector3 direction)
{
    m_direction= XMFLOAT3(direction.X, direction.Y, direction.Z);
}

XMFLOAT4 Light::GetDiffuseColor()
{
    return m_diffuseColor;
}

XMFLOAT3 Light::GetDirection()
{
    return m_direction;
}
