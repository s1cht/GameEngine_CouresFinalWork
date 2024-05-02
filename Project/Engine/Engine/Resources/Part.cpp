#include "pch.h"
#include "Part.h"

void Part::Destroy()
{
}

bool Part::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName, char* modelFileName)
{
    bool result;

    result = ResourceManager::LoadObject(fileName, m_indexCount, m_vertexCount, m_model);
    if (!result)
        return false;

    result = InitializeBuffers(device);
    if (!result)
        return false;

    result = LoadTexture(device, deviceContext, fileName);
    if (!result)
        return false;

    return true;
}

void Part::Shutdown()
{
}

void Part::Render(ID3D11DeviceContext*)
{
}

void Part::SetPosition(Vector3 position)
{
	m_position = position;
}

void Part::SetRotation(Vector3 rotation)
{
	m_rotation = rotation;
}

void Part::SetSize(Vector3 size)
{
	m_size = size;
}

Vector3 Part::GetPosition()
{
	return m_position;
}

Vector3 Part::GetRotation()
{
	return m_rotation;
}

Vector3 Part::GetSize()
{
	return m_size;
}

INT Part::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Part::GetTexture()
{
	return m_Texture->GetTexture();
}

bool Part::InitializeBuffers(ID3D11Device* device)
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

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result))
        return false;

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(size_t) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result))
        return false;

    delete[] vertices;
    vertices = nullptr;

    delete[] indices;
    indices = nullptr;

    return true;
}

void Part::ShutdownBuffers()
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

void Part::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    unsigned int stride;
    unsigned int offset;

    stride = sizeof(VertexType);
    offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Part::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName)
{
    bool result;

    m_Texture = new TextureClass;

    result = m_Texture->Initialize(device, deviceContext, fileName);
    if (!result)
        return false;

    return true;
}

void Part::ReleaseTexture()
{
    if (m_Texture)
    {
        m_Texture->Shutdown();
        delete m_Texture;
        m_Texture = nullptr;
    }
}

void Part::ReleaseModel()
{
}
