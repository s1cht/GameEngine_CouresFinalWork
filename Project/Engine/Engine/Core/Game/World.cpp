#include "pch.h"
#include "World.h"

World::World()
{
    m_Name = L"World";
    m_Parent = nullptr;
}

World::~World()
{
}

void World::SetParent(Instance* parent)
{
}

inline void World::SetName(wstring name)
{}

inline void World::SetDevices(ID3D11Device*, ID3D11DeviceContext*)
{
}

void World::AddChild(Instance* child)
{
    m_Children.push_back(child);
}

void World::DeleteChild(std::wstring childName)
{
    for (auto it = m_Children.begin(); it != m_Children.end(); it++)
        if ((*it)->GetName() == childName)
            m_Children.erase(it);
}

Instance* World::GetParent()
{
    return nullptr;
}

inline wstring World::GetName()
{
    return m_Name;
}

std::vector<Instance*> World::GetChildren()
{
    return m_Children;
}

Instance* World::operator[](const wstring childName)
{
    for (auto& child : m_Children)
        if (child->GetName() == childName)
            return child;
    return nullptr;
}

Instance* World::operator[](size_t child)
{
    if (m_Children[child])
        return m_Children[child];
    return nullptr;
}


void World::Destroy()
{}