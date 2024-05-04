#pragma once
#include "pch.h"
#include "Engine/Core/Events/Event.h"

using namespace std;

class Instance
{
public:
    Instance();

public:
    template<typename className>
    static className*& New();
    template<typename className>
    static className*& New(Instance* parent);

public:
    virtual ~Instance() = default;

    virtual void SetParent(Instance*) = 0;
    virtual void SetName(wstring) = 0;

    virtual void AddChild(Instance*) = 0;
    virtual void DeleteChild(std::wstring) = 0;

    virtual Instance*& GetParent() = 0;
    virtual wstring GetName() = 0;
    virtual std::vector<Instance*>& GetChildren() = 0;

    virtual Instance*& operator[] (const wstring) = 0;
    virtual Instance*& operator[] (size_t) = 0;

    virtual void Destroy() = 0;

protected:
    using ClassName = Instance;

    wstring m_Name;
    Instance* m_Parent;

    std::vector<Instance*> m_Children;
};

template<typename className>
className*& Instance::New()
{
    className* instance = new className;

    return instance;
}

template<typename className>
className*& Instance::New(Instance* parent)
{
    className* instance = new className;

    instance->SetParent(parent);
    parent->AddChild(instance);

    return instance;
}
