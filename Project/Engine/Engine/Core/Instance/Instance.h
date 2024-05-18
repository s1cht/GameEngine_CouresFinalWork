#pragma once
#include "pch.h"
#include "Engine/Core/Events/Event.h"


using namespace std;

class TextureFrame;

class Instance
{
public:
    virtual ~Instance() = default;

    virtual void SetParent(Instance*) = 0;
    virtual void SetName(wstring) = 0;
    virtual void SetDevices(ID3D11Device*, ID3D11DeviceContext*) = 0;

    virtual void AddChild(Instance*) = 0;
    virtual void DeleteChild(std::wstring) = 0;

    virtual Instance* GetParent() = 0;
    virtual wstring GetName() = 0;
    virtual std::vector<Instance*> GetChildren() = 0;

    virtual Instance* operator[] (const wstring) = 0;
    virtual Instance* operator[] (size_t) = 0;

    virtual void Destroy() = 0;

};

class InstanceService
{
public:
    InstanceService();

public:
    template<typename className>
    className* New();
    template<typename className>
    className* New(Instance* parent);

private:
    void SetDevice(ID3D11Device*);
    void SetDeviceContext(ID3D11DeviceContext*);
    void Shutdown();

private:
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_deviceContext;

private:
    friend class GameEngine;

};

template<typename className>
className* InstanceService::New()
{
    className* instance = new className;

    return instance;
}

template<typename className>
className* InstanceService::New(Instance* parent)
{
    className* instance = new className;

    instance->SetParent(parent);
    parent->AddChild(instance);

    return instance;
}
