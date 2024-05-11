#include "pch.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::Initialize(INT startID, INT endID)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WCHAR buffer[256];

    for (int id = startID; id <= endID; ++id) {
        int result = LoadString(hInstance, id, buffer, sizeof(buffer) / sizeof(buffer[0]));
        if (result > 0) {
            m_LoadedStrings[id] = buffer;
        }
        else {
            return false;
        }
    }

    return true;
}

void ResourceManager::Shutdown()
{
    for (auto shader : m_Shaders)
        if (shader)
        {
            shader->Shutdown();
            shader = nullptr;
        }
    for (auto texture : m_Textures)
        if (texture)
        {
            texture->Shutdown();
            texture = nullptr;
        }
    for (auto object : m_Meshes)
        if (object)
        {
            object->Shutdown();
            object = nullptr;
        }
}

bool ResourceManager::LoadObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string objectName, const WCHAR* fileName)
{
    Mesh* object;
    bool result;
    char charFileName[256];
    size_t charsConverted = 0;
    INT vertexCount; 
    INT indexCount; 
    Mesh::ModelType* objectData;

    objectData = nullptr;

    wcstombs_s(&charsConverted, charFileName, 256, fileName, 256);

    result = Mesh::ReadObjectFile(charFileName, vertexCount, indexCount, objectData);
    if (!result)
        return false;

    object = new Mesh;
    if (!object)
        return false;

    result = object->Initialize(device, deviceContext, vertexCount, indexCount, objectData);
    if (!result)
        return false;

    object->SetName(objectName);
       
    m_Meshes.push_back(object);

    return true;
}

bool ResourceManager::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string textureName, const WCHAR* fileName)
{
    Texture* texture;
    bool result;
    char charFileName[256];
    size_t charsConverted = 0;
    UCHAR* targaData; 
    Vector2 size;

    targaData = nullptr;

    wcstombs_s(&charsConverted, charFileName, 256, fileName, 256);

    result = Texture::ReadTargaFile(charFileName, targaData, size);
    if (!result)
        return false;

    texture = new Texture;
    if (!texture)
        return false;

    result = texture->Initialize(device, deviceContext, targaData, size);

    texture->SetName(textureName);

    m_Textures.push_back(texture);

    return true;
}

bool ResourceManager::LoadShader(ID3D11Device* device, HWND hwnd, std::string shaderName, const WCHAR* fileName, const WCHAR* fileName2)
{
    Shader* shader;
    bool result;
    ID3D10Blob* vertexShaderBuffer;
    ID3D10Blob* pixelShaderBuffer;

    vertexShaderBuffer = nullptr;
    pixelShaderBuffer = nullptr;

    result = Shader::ReadShaderFile(fileName, fileName2, hwnd, vertexShaderBuffer, pixelShaderBuffer);
    if (!result)
        return false;

    shader = new Shader;

    if (!shader)
        return false;

    result = shader->Initialize(device, hwnd, vertexShaderBuffer, pixelShaderBuffer);
    if (!result)
        return false;

    m_Shaders.push_back(shader);

    vertexShaderBuffer = nullptr;
    pixelShaderBuffer = nullptr;

    return true;
}

Texture* ResourceManager::GetTexture(string name)
{
    for (auto texture : m_Textures)
        if (texture)
            if (texture->GetName() == name)
            {
                return texture;
                break;
            }
    return nullptr;
}

Mesh* ResourceManager::GetMesh(string name)
{
    for (auto object : m_Meshes)
        if (object)
            if (object->GetName() == name)
            {
                return object;
                break;
            }
    return nullptr;
}

Shader* ResourceManager::GetShader(string name)
{
    for (auto shader : m_Shaders)
        if (shader)
            if (shader->GetName() == name)
            {
                return shader;
                break;
            }
    return nullptr;
}

Texture** ResourceManager::GetTextures(UINT& size)
{
    Texture** textures = new Texture * [m_Textures.size()];
    std::copy(m_Textures.begin(), m_Textures.end(), textures);
    size = m_Textures.size();
    return textures;
    textures = nullptr;
}

Mesh** ResourceManager::GetMeshes(UINT& size)
{
    Mesh** meshes = new Mesh * [m_Meshes.size()];
    std::copy(m_Meshes.begin(), m_Meshes.end(), meshes);
    size = m_Meshes.size();
    return meshes;
    meshes = nullptr;
}

Shader** ResourceManager::GetShaders(UINT& size)
{
    Shader** shaders = new Shader* [m_Shaders.size()];
    std::copy(m_Shaders.begin(), m_Shaders.end(), shaders);
    size = m_Shaders.size();
    return shaders;
    shaders = nullptr;
}

std::wstring ResourceManager::GetLoadedStringByID(INT id)
{
    auto it = m_LoadedStrings.find(id);
    if (it != m_LoadedStrings.end())
        return it->second;
    else
        return L"";
}
