#include "pch.h"
#include "World.h"

World::World()
{
    m_Name = L"World";
    m_Parent = nullptr;
    m_ID = INT();
}

World::~World()
{
}

void World::SetParent(INSTANCE parent)
{
}

inline void World::SetName(wstring name)
{}

inline void World::SetDevices(ID3D11Device*, ID3D11DeviceContext*)
{
}

void World::SetID(INT id)
{
    if (!m_idInited)
    {
        m_ID = id;
        m_idInited = true;
    }
}

void World::AddChild(INSTANCE child)
{
    m_Children.push_back(child);
}

void World::DeleteChild(INT id)
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

INSTANCE World::GetParent()
{
    return nullptr;
}

inline wstring World::GetName()
{
    return m_Name;
}

ClassName World::GetInstanceClassName()
{
    return m_ClassName;
}

std::vector<INSTANCE> World::GetChildren()
{
    return m_Children;
}

INT World::GetID()
{
    return m_ID;
}

INSTANCE World::operator[](const wstring childName)
{
    for (auto& child : m_Children)
        if (child->GetName() == childName)
            return child;
    return nullptr;
}

INSTANCE World::operator[](size_t child)
{
    if (m_Children[child])
        return m_Children[child];
    return nullptr;
}

INSTANCE World::GetChildByID(INT id)
{
    if (m_ID == id)
        return shared_from_this();

    if (m_Children.empty())
        return nullptr;

    if (id > (INT)m_Children.capacity())
        return (m_Children.end() - 1)->get()->GetChildByID(id);

    for (size_t i = 0; i < m_Children.capacity(); i++)
    {
        if (m_Children[i]->GetID() <= id)
        {
            if (m_Children[i]->GetID() == id)
                return m_Children[i];
        }
        else
        {
            return m_Children[i - 1]->GetChildByID(id);
        }
    }

    return nullptr;
}


void World::Destroy()
{}