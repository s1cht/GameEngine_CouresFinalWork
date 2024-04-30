#include "pch.h"
#include "Model.h"

ModelClass::ModelClass()
{
    m_vertexBuffer = nullptr;
    m_indexBuffer = nullptr;
    m_Texture = nullptr;
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName)
{
    bool result;

    result = InitializeBuffers(device);
    if (!result)
        return false;

    result = LoadTexture(device, deviceContext, fileName);
    if (!result)
        return false;

    return true;
}

void ModelClass::Shutdown()
{
    ReleaseTexture();
    ShutdownBuffers();
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
    RenderBuffers(deviceContext);
}

INT ModelClass::GetIndexCount()
{
    return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
    return m_Texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
    VertexType* vertices;
    size_t* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    m_vertexCount =     4;
    m_indexCount =      6;

    vertices = new VertexType[m_vertexCount];
    if (!vertices)
        return false;

    indices = new size_t[m_indexCount];
    if (!indices)
        return false;

    vertices[0].position =      XMFLOAT3(-1.f, -1.f, 0.f);
    vertices[0].texture =       XMFLOAT2(-1.f, -1.f);

    vertices[1].position =      XMFLOAT3(-1.f, 1.f, 0.f);
    vertices[1].texture =       XMFLOAT2(-1.f, 1.f);

    vertices[2].position =      XMFLOAT3(1.f, 1.f, 0.f);
    vertices[2].texture =       XMFLOAT2(1.f, 1.f);

    vertices[3].position =      XMFLOAT3(1.f, -1.f, 0.f);
    vertices[3].texture =       XMFLOAT2(1.f, -1.f);


    indices[0] =                0;
    indices[1] =                1;
    indices[2] =                2;

    indices[3] =                0;
    indices[4] =                2;
    indices[5] =                3;

    vertexBufferDesc.Usage =                        D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth =                    sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags =                    D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags =               0;
    vertexBufferDesc.MiscFlags =                    0;
    vertexBufferDesc.StructureByteStride =          0;

    vertexData.pSysMem =                            vertices;
    vertexData.SysMemPitch =                        0;
    vertexData.SysMemSlicePitch =                   0;

    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result))
        return false;

    indexBufferDesc.Usage =                         D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth =                     sizeof(size_t) * m_indexCount;
    indexBufferDesc.BindFlags =                     D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags =                0;
    indexBufferDesc.MiscFlags =                     0;
    indexBufferDesc.StructureByteStride =           0;

    indexData.pSysMem =                             indices;
    indexData.SysMemPitch =                         0;
    indexData.SysMemSlicePitch =                    0;

    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result))
        return false;
    
    delete[] vertices;
    vertices = nullptr;

    delete[] indices;
    indices = nullptr;

    return true;
}

void ModelClass::ShutdownBuffers()
{
    if (m_indexBuffer)
    {
        m_indexBuffer->Release();
        m_indexBuffer = nullptr;
    }
    if (m_vertexBuffer)
    {
        m_vertexBuffer->Release();
        m_vertexBuffer = nullptr;
    }
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    unsigned int stride;
    unsigned int offset;

    stride = sizeof(VertexType);
    offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName)
{
    bool result;

    m_Texture = new TextureClass;

    result = m_Texture->Initialize(device, deviceContext, fileName);
    if (!result)
        return false;

    return true;
}

void ModelClass::ReleaseTexture()
{
    if (m_Texture)
    {
        m_Texture->Shutdown();
        delete m_Texture;
        m_Texture = nullptr;
    }
}
