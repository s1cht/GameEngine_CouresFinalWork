#pragma once
#include "pch.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Core/Instance/Instances/TextureFrame.h"
#include "Engine/Core/Math/EngineMath.h"


class UIRenderer
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
    };

public:
    UIRenderer();
    ~UIRenderer();

    bool Initialize(ID3D11Device*, ID3D11DeviceContext*, Vector2);
    void Shutdown();
    bool Render(ID3D11DeviceContext*);

    int GetIndexCount();

private:
    bool InitializeBuffers(ID3D11Device*);
    void ShutdownBuffers();
    bool UpdateBuffers(ID3D11DeviceContext*);
    void RenderBuffers(ID3D11DeviceContext*);

private:
    ID3D11Buffer * m_vertexBuffer, * m_indexBuffer;
    INT m_vertexCount, m_indexCount;
    Vector2 m_screenSize, m_bitmapSize, m_prevPos;

};