#include "pch.h"
#include "Light.h"

Light::Light()
{
}

Light::~Light()
{
}

void Light::Destroy()
{
}

void Light::SetDiffuseColor(Color4 color)
{
    m_diffuseColor = XMFLOAT4(color.R, color.G, color.B, color.A);
}

void Light::SetAmbientColor(Color4 color)
{
    m_ambientColor = XMFLOAT4(color.R, color.G, color.B, color.A);
}

void Light::SetDirection(Vector3 direction)
{
    m_direction= XMFLOAT3(direction.X, direction.Y, direction.Z);
}

void Light::SetSpecularColor(Color4 color)
{
	m_specularColor = XMFLOAT4(color.R, color.G, color.B, color.A);
}

void Light::SetSpecularPower(FLOAT power)
{
	m_specularPower = power;
}

XMFLOAT4 Light::GetDiffuseColor()
{
    return m_diffuseColor;
}

XMFLOAT4 Light::GetAmbientColor()
{
    return m_ambientColor;
}

XMFLOAT4 Light::GetSpecularColor()
{
	return m_specularColor;
}

XMFLOAT3 Light::GetDirection()
{
    return m_direction;
}

FLOAT Light::GetSpecularPower()
{
	return m_specularPower;
}

inline void Light::SetParent(Instance* parent)
{
	m_Parent = parent;
}

inline void Light::SetName(wstring name)
{
	m_Name = name;
}

void Light::AddChild(Instance* child)
{
	m_Children.push_back(child);
}

void Light::DeleteChild(std::wstring childName)
{
	for (auto it = m_Children.begin(); it != m_Children.end(); it++)
		if ((*it)->GetName() == childName)
			m_Children.erase(it);
}

Instance*& Light::GetParent()
{
	return m_Parent;
}

wstring Light::GetName()
{
	return m_Name;
}

std::vector<Instance*>& Light::GetChildren()
{
	return m_Children;
}

Instance*& Light::operator[](const wstring childName)
{
	for (auto& child : m_Children)
		if (child->GetName() == childName)
			return child;
}

Instance*& Light::operator[](size_t child)
{
	return m_Children[child];
}