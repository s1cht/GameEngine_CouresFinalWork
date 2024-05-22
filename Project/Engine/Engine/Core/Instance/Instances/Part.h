#pragma once
#include "pch.h"
#include "Engine/Core/Instance/Instance.h"
#include "Engine/Core/Math/EngineMath.h"
#include "Engine/Resources/Texture.h"
#include "Engine/Resources/Mesh.h"

using namespace DirectX;

class Part : public Instance
{
public:
    Part();
    ~Part() override;

    void Destroy() override;

    void Initialize(std::wstring);
    void Initialize(std::wstring, INSTANCE);
    void Initialize(std::wstring, INSTANCE, Mesh*);

    void SetPosition(Vector3);
    void SetRotation(Vector3);
    void SetSize(Vector3);
    void SetPosition(FLOAT, FLOAT, FLOAT);
    void SetRotation(FLOAT, FLOAT, FLOAT);
    void SetSize(FLOAT, FLOAT, FLOAT);

    void SetColor(Color4);

    void SetMesh(Mesh*);
    void SetTexture(Texture*);

    void SetParent(INSTANCE) override;
    void SetName(std::wstring) override;
    void SetDevices(ID3D11Device*, ID3D11DeviceContext*) override;
    void SetID(INT) override;

    void AddChild(INSTANCE) override;
    void DeleteChild(INT) override;

    Mesh* GetMesh();

    Vector3 GetPosition();
    Vector3 GetRotation();
    Vector3 GetSize();

    Color4 GetColor();

    std::wstring GetName() override;
    INSTANCE GetParent() override;
    ClassName GetInstanceClassName() override;
    std::vector<INSTANCE> GetChildren() override;
    INT GetID() override;
    INSTANCE GetChildByID(INT) override;

    std::string GetMeshName();
    Texture* GetTexture();


    INSTANCE operator[] (const wstring) override;
    INSTANCE operator[] (size_t) override;

private:
    ClassName m_ClassName = PART;

    INT m_ID;

    std::wstring m_Name;
    INSTANCE m_Parent;
    bool m_idInited = false;

    Mesh* m_Mesh;
    Texture* m_Texture;

    Vector3 m_Position;
    Vector3 m_Rotation;
    Vector3 m_Size;

    Color4 m_Color;

    std::vector<INSTANCE> m_Children;

public:
    Event<void> OnDestroyed;
    Event<Vector3> Moved;
    Event<Vector3> Rotated;
    Event<Vector3> Resized;
};