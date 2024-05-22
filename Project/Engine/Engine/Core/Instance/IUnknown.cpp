#include "pch.h"
#include "IUnknown.h"

UnknownInstance::UnknownInstance()
{
    m_Name = L"IUnknown";
    m_Parent = nullptr;
    m_ID = INT();
}

UnknownInstance::~UnknownInstance()
{
}

void UnknownInstance::SetParent(INSTANCE parent)
{
    m_Parent = parent;
}

void UnknownInstance::SetName(wstring name)
{
    m_Name = name;
}

void UnknownInstance::SetDevices(ID3D11Device*, ID3D11DeviceContext*)
{
}

void UnknownInstance::SetID(INT id)
{
    if (!m_idInited)
    {
        m_ID = id;
        m_idInited = true;
    }
}

void UnknownInstance::AddChild(INSTANCE child)
{
    m_Children.push_back(child);
}

void UnknownInstance::DeleteChild(INT id)
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

inline INSTANCE UnknownInstance::GetParent()
{
    return m_Parent;
}

inline wstring UnknownInstance::GetName()
{
    return m_Name;
}

ClassName UnknownInstance::GetInstanceClassName()
{
    return m_ClassName;
}

std::vector<INSTANCE> UnknownInstance::GetChildren()
{
    return m_Children;
}

INT UnknownInstance::GetID()
{
    return m_ID;
}

INSTANCE UnknownInstance::operator[](const wstring childName)
{
    for (auto& child : m_Children)
        if (child->GetName() == childName)
            return child;
    return nullptr;
}

INSTANCE UnknownInstance::operator[](size_t child)
{
    if (m_Children[child])
        return m_Children[child];
    return nullptr;
}

INSTANCE UnknownInstance::GetChildByID(INT id)
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
void UnknownInstance::Destroy() {}