#pragma once
#include "pch.h"
#include "Engine/Core/Instance/Instance.h"

class UnknownInstance :
    public Instance
{
public:
    UnknownInstance();
    ~UnknownInstance() override;

    void SetParent(INSTANCE) override;
    void SetName(wstring) override;
    void SetDevices(ID3D11Device*, ID3D11DeviceContext*) override;
    void SetID(INT) override;

    void AddChild(INSTANCE) override;
    void DeleteChild(INT) override;

    INSTANCE GetParent() override;
    wstring GetName() override;
    ClassName GetInstanceClassName() override;
    std::vector<INSTANCE> GetChildren() override;
    INT GetID() override;
    INSTANCE GetChildByID(INT) override;

    INSTANCE operator[] (const wstring) override;
    INSTANCE operator[] (size_t) override;

    void Destroy() override;

private:
    ClassName m_ClassName = UNKNOWN;

    INT m_ID;

    wstring m_Name;
    INSTANCE m_Parent;
    bool m_idInited = false;

    std::vector<INSTANCE> m_Children;
};