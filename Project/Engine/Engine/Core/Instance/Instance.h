#pragma once
#include "pch.h"
#include "Engine/Core/Events/Event.h"

using namespace std;

enum ClassName {
    CAMERA,
    WORLD,
    SUNLIGHT,
    PART,
    UNKNOWN
};

class Instance;

typedef ENG_PTR<Instance> INSTANCE;

class Instance : public std::enable_shared_from_this<Instance>
{
public:
    virtual ~Instance() = default;

    virtual void SetParent(INSTANCE) = 0;
    virtual void SetName(wstring) = 0;
    virtual void SetDevices(ID3D11Device*, ID3D11DeviceContext*) = 0;
    virtual void SetID(INT) = 0;

    virtual void AddChild(INSTANCE) = 0;
    virtual void DeleteChild(INT) = 0;

    virtual INSTANCE GetParent() = 0;
    virtual wstring GetName() = 0;
    virtual ClassName GetInstanceClassName() = 0;
    virtual std::vector<INSTANCE> GetChildren() = 0;
    virtual INT GetID() = 0;
    virtual INSTANCE GetChildByID(INT) = 0;

    virtual INSTANCE operator[] (const wstring) = 0;
    virtual INSTANCE operator[] (size_t) = 0;

    virtual void Destroy() = 0;

};

class InstanceService
{
public:
    InstanceService();

public:
    template<typename className>
    ENG_PTR<className> New();
    template<typename className>
    ENG_PTR<className> New(INSTANCE parent);
    INT GetInstanceCount();

    static std::wstring ConvertClassNameToWString(ClassName);
    static void SetParent(INSTANCE, INSTANCE);
private:
    void SetDevice(ID3D11Device*);
    void SetDeviceContext(ID3D11DeviceContext*);
    void Shutdown();


    template<typename className>
    ENG_PTR<className> New(INSTANCE parent, INT id);
    template<typename className>
    ENG_PTR<className> New(INT id);

private:
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_deviceContext;

    INT m_instanceCount = 0;

private:
    friend class GameEngine;

};

template<typename className>
ENG_PTR<className> InstanceService::New()
{
    ENG_PTR<className> instance = TO_ENG_PTR<className>();

    m_instanceCount++;

    instance->SetID(m_instanceCount);

    return instance;
}

template<typename className>
ENG_PTR<className> InstanceService::New(INSTANCE parent)
{
    ENG_PTR<className> instance = TO_ENG_PTR<className>();

    m_instanceCount++;

    instance->SetID(m_instanceCount);

    SetParent(parent, instance);

    return instance;
}

template<typename className>
inline ENG_PTR<className> InstanceService::New(INSTANCE parent, INT id)
{
    ENG_PTR<className> instance = TO_ENG_PTR<className>();

    m_instanceCount++;

    instance->SetID(id);

    SetParent(parent, instance);

    return instance;
}

template<typename className>
inline ENG_PTR<className> InstanceService::New(INT id)
{
    ENG_PTR<className> instance = TO_ENG_PTR<className>();

    m_instanceCount++;

    instance->SetID(id);

    return instance;
}
