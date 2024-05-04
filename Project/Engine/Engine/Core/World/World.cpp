#include "pch.h"
#include "World.h"

World::World()
{
}

World::~World()
{
}

void World::SetParent(Instance* parent)
{
}

inline void World::SetName(wstring name)
{
    m_Name = name;
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

inline Instance*& World::GetParent()
{
    return m_Parent;
}

inline wstring World::GetName()
{
    return m_Name;
}

std::vector<Instance*>& World::GetChildren()
{
    return m_Children;
}

Instance*& World::operator[](const wstring childName)
{
    for (auto& child : m_Children)
        if (child->GetName() == childName)
            return child;
}

Instance*& World::operator[](size_t child)
{
    return m_Children[child];
}


void World::Destroy()
{

}