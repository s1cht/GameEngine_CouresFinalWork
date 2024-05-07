#include "pch.h"
#include "Mesh.h"

Mesh::Mesh()
{
    m_vertexBuffer = nullptr;
    m_indexBuffer = nullptr;
    m_model = nullptr;
}

Mesh::~Mesh()
{
}

bool Mesh::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, INT vertexCount, INT indexCount, ModelType* objectData)
{
    bool result;

    m_vertexCount = vertexCount;
    m_indexCount = indexCount;
    m_model = objectData;

    result = InitializeBuffers(device);
    if (!result)
        return false;

    return true;
}

void Mesh::Shutdown()
{
    ReleaseTexture();
    ReleaseModel();
    ShutdownBuffers();
}

void Mesh::Render(ID3D11DeviceContext* deviceContext)
{
    RenderBuffers(deviceContext);
}

void Mesh::SetName(std::string name)
{
    m_name = name;
}

INT Mesh::GetIndexCount()
{
    return m_indexCount;
}

std::string Mesh::GetName()
{
    return m_name;
}

bool Mesh::InitializeBuffers(ID3D11Device* device)
{
    VertexType* vertices;
    size_t* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    vertices = new VertexType[m_vertexCount];
    if (!vertices)
        return false;

    indices = new size_t[m_indexCount];
    if (!indices)
        return false;

    for (INT i = 0; i < m_vertexCount; i++)
    {
        vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
        vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
        vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

        indices[i] = i;
    }

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

void Mesh::ShutdownBuffers()
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

void Mesh::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    unsigned int stride;
    unsigned int offset;

    stride = sizeof(VertexType);
    offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Mesh::ReleaseTexture()
{
    //m_Texture = nullptr;
}

void Mesh::ReleaseModel()
{
    if (m_model)
    {
        delete[] m_model;
        m_model = nullptr;
    }
}

bool Mesh::ReadObjectFile(const char* fileName, INT& vertexCount, INT& indexCount, ModelType*& object)
{
    std::ifstream file;
    char input;
    int i;

    file.open(fileName);

    if (file.fail())
        return false;

    file.get(input);
    while (input != ':')
        file.get(input);

    file >> vertexCount;

    indexCount = vertexCount;

    object = new ModelType[vertexCount];

    if (!object)
        return false;

    file.get(input);

    while (input != ':')
        file.get(input);

    file.get(input);
    file.get(input);

    for (i = 0; i < vertexCount; i++)
    {
        file >> object[i].x >> object[i].y >> object[i].z;
        file >> object[i].tu >> object[i].tv;
        file >> object[i].nx >> object[i].ny >> object[i].nz;
    }

    file.close();

    return true;
}
