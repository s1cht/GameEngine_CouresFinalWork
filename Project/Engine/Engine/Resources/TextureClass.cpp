#include "pch.h"
#include "TextureClass.h"

TextureClass::TextureClass()
{
    m_TargaData = nullptr;
    m_texture = nullptr;
    m_textureView = nullptr;
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName)
{
    bool result;
    D3D11_TEXTURE2D_DESC textureDesc;
    HRESULT hResult;
    UINT rowPitch;
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

    result = LoadTarga32B(fileName);
    if (!result)
        return false;

    textureDesc.Height = (UINT)m_size.X;
    textureDesc.Width = (UINT)m_size.Y;
    textureDesc.MipLevels = 0;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
    if (FAILED(hResult))
        return false;

    rowPitch = ((UINT)m_size.X * 4) * sizeof(UCHAR);

    deviceContext->UpdateSubresource(m_texture, 0, NULL, m_TargaData, rowPitch, 0);

    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = -1;

    hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
    if (FAILED(hResult))
        return false;

    deviceContext->GenerateMips(m_textureView);

    delete[] m_TargaData;
    m_TargaData = nullptr;

    return true;
}

void TextureClass::Shutdown()
{
    if (m_textureView)
    {
        m_textureView->Release();
        m_textureView = nullptr;
    }
    if (m_texture)
    {
        m_texture->Release();
        m_texture = nullptr;
    }
    if (m_TargaData)
    {
        delete[] m_TargaData;
        m_TargaData = nullptr;
    }
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
    return m_textureView;
}

INT TextureClass::GetWidth()
{
    return (INT)m_size.X;
}

INT TextureClass::GetHeight()
{
    return (INT)m_size.Y;
}

bool TextureClass::LoadTarga32B(char* fileName)
{
    INT error, bpp, imageSize, index, i, j, k;
    FILE* filePtr;
    UINT count;
    TargaHeader targaFileHeader;
    UCHAR* targaImage;

    error = fopen_s(&filePtr, fileName, "rb");
    if (error != 0)
        return false;

    count = (UINT)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
    if (count != 1)
        return false;

    m_size.X = (FLOAT)targaFileHeader.width;
    m_size.Y = (FLOAT)targaFileHeader.height;
    bpp = (INT)targaFileHeader.bpp;

    if (bpp != 32)
        return false;

    imageSize = (INT)m_size.X * (INT)m_size.Y * 4;

    targaImage = new UCHAR[imageSize];

    count = (UINT)fread(targaImage, 1, imageSize, filePtr);
    if (count != imageSize)
        return false;

    error = fclose(filePtr);
    if (error != 0)
        return false;

    m_TargaData = new UCHAR[imageSize];

    index = 0;

    k = imageSize - ((INT)m_size.X * 4);

    for (j = 0; j < (INT)m_size.Y; j++)
    {
        for (i = 0; i < (INT)m_size.X; i++)
        {
            m_TargaData[index + 0] = targaImage[k + 2];
            m_TargaData[index + 1] = targaImage[k + 1];
            m_TargaData[index + 2] = targaImage[k + 0];
            m_TargaData[index + 3] = targaImage[k + 3];

            k +=        4;
            index +=    4;
        }
        
        k -= (INT)(m_size.X * 8);
    }

    delete[] targaImage;
    targaImage = nullptr;

    return true;
}
