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

void Part::Initialize(std::wstring name, Instance* parent)
{
	m_Name =		name;
	m_Parent =		parent;
}

void Part::Initialize(std::wstring name, Instance* parent, Mesh* mesh)
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

inline void Part::SetParent(Instance* parent)
{
	m_Parent =		parent;
}

inline void Part::SetName(wstring name)
{
	m_Name =		name;
}

void Part::AddChild(Instance* child)
{
	m_Children.push_back(child);
}

void Part::DeleteChild(std::wstring childName)
{
	for (auto it = m_Children.begin(); it != m_Children.end(); it++)
		if ((*it)->GetName() == childName)
			m_Children.erase(it);
}

void Part::SetMesh(Mesh* mesh)
{
	m_Mesh =		mesh;
}

void Part::SetTexture(Texture* texture)
{
	m_Texture =		texture;
}

Instance* Part::GetParent()
{
	return m_Parent;
}

inline std::vector<Instance*> Part::GetChildren()
{
	return m_Children;
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

Instance* Part::operator[](const wstring childName)
{
	for (auto& child : m_Children)
		if (child->GetName() == childName)
			return child;
	return nullptr;
}

Instance* Part::operator[](size_t child)
{
	if (m_Children[child])
		return m_Children[child];
	return nullptr;
}