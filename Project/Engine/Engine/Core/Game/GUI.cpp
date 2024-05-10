#include "pch.h"
#include "GUI.h"

GUI::GUI()
{
    m_Name = L"GUI";
    m_Parent = nullptr;
}

GUI::~GUI()
{
}

void GUI::SetParent(Instance* parent) {}

void GUI::SetName(wstring name) {}

void GUI::AddChild(Instance* child)
{
    m_Children.push_back(child);
}

void GUI::DeleteChild(std::wstring childName)
{
    for (auto it = m_Children.begin(); it != m_Children.end(); it++)
        if ((*it)->GetName() == childName)
            m_Children.erase(it);
}

inline Instance* GUI::GetParent() {}

inline wstring GUI::GetName()
{
    return m_Name;
}

std::vector<Instance*> GUI::GetChildren()
{
    return m_Children;
}

Instance* GUI::operator[](const wstring childName)
{
    for (auto& child : m_Children)
        if (child->GetName() == childName)
            return child;
    return nullptr;
}

Instance* GUI::operator[](size_t child)
{
    if (m_Children[child])
        return m_Children[child];
    return nullptr;
}

void GUI::Destroy() {}