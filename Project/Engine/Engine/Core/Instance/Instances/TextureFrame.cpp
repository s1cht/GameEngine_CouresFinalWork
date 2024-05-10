#include "pch.h"
#include "TextureFrame.h"

TextureFrame::TextureFrame()
{
	m_Parent = nullptr;
	m_Texture = nullptr;
	m_Size = Vector2::Zero();
	m_Position = Vector2::Zero();
	m_Rotation = Vector2::Zero();
	m_Color = Color4{ 1.f, 1.f, 1.f, 1.f };
	m_Name = L"Frame";
}

TextureFrame::~TextureFrame()
{
}

void TextureFrame::Destroy()
{
	m_Parent = nullptr;
	for (auto& child : m_Children)
	{
		child->Destroy();
		child = nullptr;
	}
}

void TextureFrame::Initialize(std::wstring name)
{
	m_Name = name;
}

void TextureFrame::Initialize(std::wstring name, Instance* parent)
{
	m_Name = name;
	m_Parent = parent;
}

void TextureFrame::Initialize(std::wstring name, Instance* parent, Texture* texture)
{
	m_Name = name;
	m_Parent = parent;
	m_Texture = texture;
}

void TextureFrame::SetPosition(Vector2 position)
{
	m_Position = position;
}

void TextureFrame::SetPosition(FLOAT x, FLOAT y)
{
	m_Position = Vector2{ x, y };
}

void TextureFrame::SetRotation(Vector2 rotation)
{
	m_Rotation = rotation;
}

void TextureFrame::SetRotation(FLOAT x, FLOAT y)
{
	m_Rotation = Vector2{ x, y };
}

void TextureFrame::SetSize(Vector2 size)
{
	m_Size = size;
}

void TextureFrame::SetSize(FLOAT x, FLOAT y)
{
	m_Size = Vector2{ x, y };
}

void TextureFrame::SetColor(Color4 color)
{
	m_Color = color;
}

Vector2 TextureFrame::GetPosition()
{
	return m_Position;
}

Vector2 TextureFrame::GetRotation()
{
	return m_Rotation;
}

Vector2 TextureFrame::GetSize()
{
	return m_Size;
}

Color4 TextureFrame::GetColor()
{
	return m_Color;
}

inline void TextureFrame::SetParent(Instance* parent)
{
	m_Parent = parent;
}

inline void TextureFrame::SetName(wstring name)
{
	m_Name = name;
}

void TextureFrame::AddChild(Instance* child)
{
	m_Children.push_back(child);
}

void TextureFrame::DeleteChild(std::wstring childName)
{
	for (auto it = m_Children.begin(); it != m_Children.end(); it++)
		if ((*it)->GetName() == childName)
			m_Children.erase(it);
}

void TextureFrame::SetTexture(Texture* texture)
{
	m_Texture = texture;
}

Instance* TextureFrame::GetParent()
{
	return m_Parent;
}

inline std::vector<Instance*> TextureFrame::GetChildren()
{
	return m_Children;
}

inline wstring TextureFrame::GetName()
{
	return m_Name;
}

Texture* TextureFrame::GetTexture()
{
	return m_Texture;
}

Instance* TextureFrame::operator[](const wstring childName)
{
	for (auto& child : m_Children)
		if (child->GetName() == childName)
			return child;
	return nullptr;
}

Instance* TextureFrame::operator[](size_t child)
{
	if (m_Children[child])
		return m_Children[child];
	return nullptr;
}
