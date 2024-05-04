#pragma once
#include "pch.h"
#include "Engine/Core/Instance/Instance.h"
#include "Engine/Core/Math/EngineMath.h"
#include "Engine/Resources/Texture.h"
#include "Engine/Resources/ResourceManager.h"

using namespace DirectX;

class Part : public Instance
{
public:
    Part();
    ~Part() override;

    void Destroy() override;

    void Initialize(std::wstring);
    void Initialize(std::wstring, Instance*);
    void Initialize(std::wstring, Instance*, Mesh*);

    void SetPosition(Vector3);
    void SetRotation(Vector3);
    void SetSize(Vector3);
    void SetPosition(FLOAT, FLOAT, FLOAT);
    void SetRotation(FLOAT, FLOAT, FLOAT);
    void SetSize(FLOAT, FLOAT, FLOAT);

    void SetMesh(Mesh*);
    void SetChangeTexture(Texture*);

    void SetParent(Instance*) override;
    void SetName(std::wstring) override;

    void AddChild(Instance*) override;
    void DeleteChild(std::wstring) override;

    Mesh* GetMesh();
    Vector3 GetPosition();
    Vector3 GetRotation();
    Vector3 GetSize();

    std::wstring GetName() override;
    Instance*& GetParent() override;
    std::vector<Instance*>& GetChildren() override;

    std::string GetMeshName();

    Instance*& operator[] (const wstring) override;
    Instance*& operator[] (size_t) override;

private:
    std::string m_ClassName = "Part";

    std::wstring m_Name;
    Instance* m_Parent;
    Mesh* m_Mesh;

    Vector3 m_Position;
    Vector3 m_Rotation;
    Vector3 m_Size;

    std::vector<Instance*> m_Children;

public:
    Event<void> OnDestroyed;
    Event<Vector3> Moved;
    Event<Vector3> Rotated;
    Event<Vector3> Resized;
};