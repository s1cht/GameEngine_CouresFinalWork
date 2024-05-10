#include "UIRenderer.h"

UIRenderer::UIRenderer()
{
    m_vertexBuffer = nullptr;
    m_indexBuffer = nullptr;
}

UIRenderer::~UIRenderer()
{
}

bool UIRenderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Vector2 screenSize)
{
    bool result;

    m_screenSize = screenSize;

    result = InitializeBuffers(device);
    if (!result)
        return false;

    return true;
}

void UIRenderer::Shutdown()
{
    ShutdownBuffers();
}

bool UIRenderer::Render(ID3D11DeviceContext* deviceContext)
{
    bool result;

    result = UpdateBuffers(deviceContext);
    if (!result)
        return false;

    RenderBuffers(deviceContext);

    return true;
}

int UIRenderer::GetIndexCount()
{
    return m_indexCount;
}

bool UIRenderer::InitializeBuffers(ID3D11Device* device)
{
    return true;
}

void UIRenderer::ShutdownBuffers()
{
}

bool UIRenderer::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
    return true;
}

void UIRenderer::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
}