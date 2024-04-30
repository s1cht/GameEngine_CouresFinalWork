#include "pch.h"
#include "Model.h"

ModelClass::ModelClass()
{
    m_vertexBuffer = nullptr;
    m_indexBuffer = nullptr;
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device)
{
    bool result;

    result = InitializeBuffers(device);
    if (!result)
        return false;

    return true;
}

void ModelClass::Shutdown()
{
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

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
    VertexType* vertices;
    size_t* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    m_vertexCount =     3;
    m_indexCount =      3;

    vertices = new VertexType[m_vertexCount];
    if (!vertices)
        return false;

    indices = new size_t[m_indexCount];
    if (!indices)
        return false;

    vertices[0].position =      XMFLOAT3(-1.f, -1.f, 0.f);
    vertices[0].color =         XMFLOAT4(0.f, 1.f, 0.f, 1.f);

    vertices[1].position =      XMFLOAT3(0.f, 1.f, 0.f);
    vertices[1].color =         XMFLOAT4(0.f, 1.f, 0.f, 1.f);

    vertices[2].position =      XMFLOAT3(1.f, -1.f, 0.f);
    vertices[2].color =         XMFLOAT4(0.f, 1.f, 0.f, 1.f);

    indices[0] =                0;
    indices[1] =                1;
    indices[2] =                2;

    vertexBufferDesc.Usage =                        D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth =                    sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags =                    D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags =               0;
    vertexBufferDesc.MiscFlags =                    0;
    vertexBufferDesc.StructureByteStride =          0;

    vertexData.pSysMem =                            indices;
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
    size_t stride;
    size_t offset;

    stride = sizeof(VertexType);
    offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
