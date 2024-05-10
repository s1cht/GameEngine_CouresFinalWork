#pragma once
#include "pch.h"
#include "Engine/Core/Instance/Instance.h"

class GUI :
    public Instance
{
    GUI();
    ~GUI() override;

    void SetParent(Instance*) override;
    void SetName(wstring) override;

    void AddChild(Instance*) override;
    void DeleteChild(std::wstring) override;

    Instance* GetParent() override;
    wstring GetName() override;
    std::vector<Instance*> GetChildren() override;

    Instance* operator[] (const wstring) override;
    Instance* operator[] (size_t) override;

    void Destroy() override;

private:
    using ClassName = GUI;

    wstring m_Name;
    Instance* m_Parent;

    std::vector<Instance*> m_Children;
};

