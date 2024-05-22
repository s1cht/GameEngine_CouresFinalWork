#include "pch.h"
#include "Part.h"

Part::Part()
{
	m_Mesh =		nullptr;
	m_Parent =		nullptr;
	m_Texture =		nullptr;
	m_Size =		Vector3::Zero();
	m_Position =	Vector3::Zero();
	m_Rotation =	Vector3::Zero();
	m_Color =		Color4{ 0.5f, 0.5f, 0.5f, 1.f };
	m_Name =		L"Part";
	m_ID =			INT();
}

Part::~Part()
{
}

void Part::Destroy()
{
	m_Parent = nullptr;
	m_Mesh = nullptr;
	for (auto &child : m_Children)
	{
		child->Destroy();
		child = nullptr;
	}
}

void Part::Initialize(std::wstring name)
{
	m_Name =		name;
}

void Part::Initialize(std::wstring name, INSTANCE parent)
{
	m_Name =		name;
	m_Parent =		parent;
}

void Part::Initialize(std::wstring name, INSTANCE parent, Mesh* mesh)
{
	m_Name =		name;
	m_Parent =		parent;
	m_Mesh =		mesh;
}

void Part::SetPosition(Vector3 position)
{
	m_Position =	position;
}

void Part::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
	m_Position =	Vector3{ x, y, z };
}

void Part::SetRotation(Vector3 rotation)
{
	m_Rotation =	rotation;
}

void Part::SetRotation(FLOAT x, FLOAT y, FLOAT z)
{
	m_Rotation =	Vector3{ x, y, z };
}

void Part::SetSize(Vector3 size)
{
	m_Size =		size;
}

void Part::SetSize(FLOAT x, FLOAT y, FLOAT z)
{
	m_Size =		Vector3{ x, y, z };
}

void Part::SetColor(Color4 color)
{
	m_Color =		color;
}

Mesh* Part::GetMesh()
{
	return m_Mesh;
}

Vector3 Part::GetPosition()
{
	return m_Position;
}

Vector3 Part::GetRotation()
{
	return m_Rotation;
}

Vector3 Part::GetSize()
{
	return m_Size;
}

Color4 Part::GetColor()
{
	return m_Color;
}

inline void Part::SetParent(INSTANCE parent)
{
	m_Parent =		parent;
}

inline void Part::SetName(wstring name)
{
	m_Name =		name;
}

inline void Part::SetDevices(ID3D11Device*, ID3D11DeviceContext*)
{
}

void Part::SetID(INT id)
{
	if (!m_idInited)
	{
		m_ID = id;
		m_idInited = true;
	}
}

void Part::AddChild(INSTANCE child)
{
	m_Children.push_back(child);
}

void Part::DeleteChild(INT id)
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

void Part::SetMesh(Mesh* mesh)
{
	m_Mesh =		mesh;
}

void Part::SetTexture(Texture* texture)
{
	m_Texture =		texture;
}

INSTANCE Part::GetParent()
{
	return m_Parent;
}

ClassName Part::GetInstanceClassName()
{
	return m_ClassName;
}

inline std::vector<INSTANCE> Part::GetChildren()
{
	return m_Children;
}

INT Part::GetID()
{
	return m_ID;
}

inline wstring Part::GetName()
{
	return m_Name;
}

std::string Part::GetMeshName()
{
	return m_Mesh->GetName();
}

Texture* Part::GetTexture()
{
	return m_Texture;
}

INSTANCE Part::operator[](const wstring childName)
{
	for (auto& child : m_Children)
		if (child->GetName() == childName)
			return child;
	return nullptr;
}

INSTANCE Part::operator[](size_t child)
{
	if (m_Children[child])
		return m_Children[child];
	return nullptr;
}

INSTANCE Part::GetChildByID(INT id)
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