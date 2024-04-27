#include "pch.h"
#include "ColorShader.h"

ColorShader::ColorShader()
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
}

ColorShader::ColorShader(const ColorShader&)
{
}

ColorShader::~ColorShader()
{
}

bool ColorShader::Initialize(ID3D11Device*, HWND)
{
	return false;
}

void ColorShader::Shutdown()
{
}

bool ColorShader::Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX)
{
	return false;
}

bool ColorShader::InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*)
{
	return false;
}

void ColorShader::ShutdownShader()
{
}

void ColorShader::OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*)
{
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX)
{
	return false;
}

void ColorShader::RenderShader(ID3D11DeviceContext*, int)
{
}
