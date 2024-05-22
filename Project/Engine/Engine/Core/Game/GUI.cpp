#include "pch.h"
#include "GUI.h"

GUI::GUI()
{
    m_Name = L"GUI";
    m_Parent = nullptr;
    m_ID = INT();
}

GUI::~GUI()
{
}

void GUI::SetParent(INSTANCE parent) {}

void GUI::SetName(wstring name) {}

void GUI::SetID(INT id)
{
    if (!m_idInited)
    {
        m_ID = id;
        m_idInited = true;
    }
}

void GUI::AddChild(INSTANCE child)
{
    m_Children.push_back(child);
}

void GUI::DeleteChild(INT id)
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

inline INSTANCE GUI::GetParent() 
{
    return nullptr;
}

inline wstring GUI::GetName()
{
    return m_Name;
}

ClassName GUI::GetInstanceClassName()
{
    return m_ClassName;
}

std::vector<INSTANCE> GUI::GetChildren()
{
    return m_Children;
}

INT GUI::GetID()
{
    return m_ID;
}

INSTANCE GUI::operator[](const wstring childName)
{
    for (auto& child : m_Children)
        if (child->GetName() == childName)
            return child;
    return nullptr;
}

INSTANCE GUI::operator[](size_t child)
{
    if (m_Children[child])
        return m_Children[child];
    return nullptr;
}

INSTANCE GUI::GetChildByID(INT id)
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
void GUI::Destroy() {}