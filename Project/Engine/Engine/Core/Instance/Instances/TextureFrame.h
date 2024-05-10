#pragma once

#include "pch.h"
#include "Engine/Core/Instance/Instance.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Resources/Texture.h"
#include "Engine/Graphics/GraphicsEngine.h"
#include "Engine/Core/Math/EngineMath.h"

class TextureFrame :
	public Instance
{
public:
    TextureFrame();
    ~TextureFrame() override;

    void Initialize(std::wstring);
    void Initialize(std::wstring, Instance*);
    void Initialize(std::wstring, Instance*, Texture*);
    bool Initialize(GraphicsEngine*);

    void SetPosition(Vector2);
    void SetRotation(Vector2);
    void SetSize(Vector2);
    void SetPosition(FLOAT, FLOAT);
    void SetRotation(FLOAT, FLOAT);
    void SetSize(FLOAT, FLOAT);

    void SetColor(Color4);

    void SetTexture(Texture*);

    void SetParent(Instance*) override;
    void SetName(wstring) override;

    void AddChild(Instance*) override;
    void DeleteChild(std::wstring) override;

    Vector2 GetPosition();
    Vector2 GetRotation();
    Vector2 GetSize();

    Color4 GetColor();

    Texture* GetTexture();

    Instance* GetParent() override;
    wstring GetName() override;
    std::vector<Instance*> GetChildren() override;

    Instance* operator[] (const wstring) override;
    Instance* operator[] (size_t) override;

    void Destroy() override;

    void Shutdown();

    void Render(ID3D11DeviceContext*);

    void SetName(std::string);

    INT GetIndexCount();
    std::string GetName();

private:
    bool InitializeBuffers(ID3D11Device*);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext*);

private:
    std::string m_ClassName = "TextureFrame";

    std::wstring m_Name;
    Instance* m_Parent;

    Texture* m_Texture;

    Vector2 m_Position;
    Vector2 m_Rotation;
    Vector2 m_Size;

    Color4 m_Color;

    std::vector<Instance*> m_Children;

private:
    ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
    INT m_vertexCount, m_indexCount;

};