#include "pch.h"
#include "Light.h"

Light::Light()
{
	m_ambientColor = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.f);
	m_diffuseColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_specularColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	m_direction = XMFLOAT3( 1.f,0.f,0.f );
	m_specularPower = 32.f;
	m_Name = L"SunLight";
	m_Parent = nullptr;
	m_ID = INT();
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

inline void Light::SetParent(INSTANCE parent)
{
	m_Parent = parent;
}

inline void Light::SetName(wstring name)
{
	m_Name = name;
}

inline void Light::SetDevices(ID3D11Device*, ID3D11DeviceContext*)
{
}

void Light::SetID(INT id)
{
	if (!m_idInited)
	{
		m_ID = id;
		m_idInited = true;
	}
}

void Light::AddChild(INSTANCE child)
{
	m_Children.push_back(child);
}

void Light::DeleteChild(INT id)
{
	if (m_Children.empty())
		return;
	for (auto it = m_Children.begin(); it != m_Children.end(); )
		if ((*it)->GetID() == id)
		{
			m_Children.erase(it);
			break;
		}
		else
			it++;
}

INSTANCE Light::GetParent()
{
	return m_Parent;
}

wstring Light::GetName()
{
	return m_Name;
}

std::vector<INSTANCE> Light::GetChildren()
{
	return m_Children;
}

ClassName Light::GetInstanceClassName()
{
	return m_ClassName;
}

INT Light::GetID()
{
	return m_ID;
}

INSTANCE Light::operator[](const wstring childName)
{
	for (auto& child : m_Children)
		if (child->GetName() == childName)
			return child;
	return nullptr;
}

INSTANCE Light::operator[](size_t child)
{
	if (m_Children[child])
		return m_Children[child];
	return nullptr;
}

INSTANCE Light::GetChildByID(INT id)
{
	if (m_ID == id)
		return shared_from_this();

	if (m_Children.empty())
		return nullptr;

	if (id > (INT)m_Children.capacity())
		return (m_Children.end() - 1)->get()->GetChildByID(id);

	for (size_t i = 0; i < m_Children.capacity(); i++)
	{
		if (m_Children[i]->GetID() < id)
		{
			if (m_Children[i]->GetID() == id)
				return m_Children[i];
		}
		else if (m_Children[i]->GetID() == id)
			return m_Children[i];
		else
		{
			return m_Children[i - 1]->GetChildByID(id);
		}
	}

	return nullptr;
}