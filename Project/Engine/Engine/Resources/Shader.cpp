#include "pch.h"
#include "Shader.h"

Shader::Shader()
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_layout = nullptr;
	m_matrixBuffer = nullptr;
	m_sampleState = nullptr;
	m_lightBuffer = nullptr;
	m_cameraBuffer = nullptr;
}

Shader::Shader(const Shader&)
{
}

Shader::~Shader()
{
}

bool Shader::Initialize(ID3D11Device* device, HWND hwnd, ID3D10Blob** vertexShaderBuffer, ID3D10Blob** pixelShaderBuffer)
{
	bool result;

	result = InitializeShader(device, hwnd, vertexShaderBuffer, pixelShaderBuffer);
	if (!result)
		return false;

	return true;
}

void Shader::Shutdown()
{
	ShutdownShader();
}

bool Shader::Render(ID3D11DeviceContext* deviceContext, INT indexCount, 
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, 
	ID3D11ShaderResourceView* texture, 
	XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, FLOAT specularPower)
{
	bool result;

	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, diffuseColor, ambientColor, cameraPosition, specularColor, specularPower);
	if (!result)
		return false;

	RenderShader(deviceContext, indexCount);

	return true;
}

void Shader::SetName(string Name)
{
	m_name = Name;
}

string Shader::GetName()
{
	return m_name;
}

bool Shader::InitializeShader(ID3D11Device* device, HWND hwnd, ID3D10Blob** vertexShaderBuffer, ID3D10Blob** pixelShaderBuffer)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	UINT numElements;

	result = device->CreateVertexShader((*vertexShaderBuffer)->GetBufferPointer(), (*vertexShaderBuffer)->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
		return false;

	result = device->CreatePixelShader((*pixelShaderBuffer)->GetBufferPointer(), (*pixelShaderBuffer)->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
		return false;

	polygonLayout[0].SemanticName =			"POSITION";
	polygonLayout[0].SemanticIndex =		0;
	polygonLayout[0].Format =				DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot =			0;
	polygonLayout[0].AlignedByteOffset =	0;
	polygonLayout[0].InputSlotClass =		D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName =			"TEXCOORD";
	polygonLayout[1].SemanticIndex =		0;
	polygonLayout[1].Format =				DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot =			0;
	polygonLayout[1].AlignedByteOffset =	D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass =		D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName =			"NORMAL";
	polygonLayout[2].SemanticIndex =		0;
	polygonLayout[2].Format =				DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot =			0;
	polygonLayout[2].AlignedByteOffset =	D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass =		D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(polygonLayout, numElements, (*vertexShaderBuffer)->GetBufferPointer(), (*vertexShaderBuffer)->GetBufferSize(), &m_layout);
	if (FAILED(result))
		return false;

	(*vertexShaderBuffer)->Release();
	vertexShaderBuffer = nullptr;

	(*pixelShaderBuffer)->Release();
	pixelShaderBuffer = nullptr;

	matrixBufferDesc.Usage =				D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth =			sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags =			D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags =		D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags =			0;
	matrixBufferDesc.StructureByteStride =	0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
		return false;

	samplerDesc.Filter =			D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU =			D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV =			D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW =			D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias =		0.f;
	samplerDesc.MaxAnisotropy =		1;
	samplerDesc.ComparisonFunc =	D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] =	0;
	samplerDesc.BorderColor[1] =	0;
	samplerDesc.BorderColor[2] =	0;
	samplerDesc.BorderColor[3] =	0;
	samplerDesc.MinLOD =			0;
	samplerDesc.MaxLOD =			D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
		return false;

	cameraBufferDesc.Usage =				D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth =			sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags =			D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags =		D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags =			0;
	cameraBufferDesc.StructureByteStride =	0;

	result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
	if (FAILED(result))
		return false;

	lightBufferDesc.Usage =					D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth =				sizeof(LightBufferType);
	lightBufferDesc.BindFlags =				D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags =		D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags =				0;
	lightBufferDesc.StructureByteStride =	0;

	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
		return false;

	return true;
}

void Shader::ShutdownShader()
{
	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = nullptr;
	}
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = nullptr;
	}
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = nullptr;
	}
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}
	if (m_layout)
	{
		m_layout->Release();
		m_layout = nullptr;
	}
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}
}

void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	size_t bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());

	bufferSize = errorMessage->GetBufferSize();

	fout.open("shader_error.txt");

	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	errorMessage->Release();
	errorMessage = nullptr;

	MessageBox(hwnd, L"Failed to compile shaders. Check shader_error.txt for information.", shaderFilename, MB_OK);
}

bool Shader::SetShaderParameters(ID3D11DeviceContext* deviceContext, 
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, 
	ID3D11ShaderResourceView* texture, 
	XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, FLOAT specularPower)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	CameraBufferType* dataPtr3;
	UINT bufferNumber;

	worldMatrix =		XMMatrixTranspose(worldMatrix);
	viewMatrix =		XMMatrixTranspose(viewMatrix);
	projectionMatrix =	XMMatrixTranspose(projectionMatrix);

	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world			= worldMatrix;
	dataPtr->view			= viewMatrix;
	dataPtr->projection		= projectionMatrix;

	deviceContext->Unmap(m_matrixBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	deviceContext->PSSetShaderResources(0, 1, &texture);


	result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr3 = (CameraBufferType*)mappedResource.pData;

	dataPtr3->cameraPosition = cameraPosition;
	dataPtr3->padding = 0.0f;

	deviceContext->Unmap(m_cameraBuffer, 0);

	bufferNumber = 1;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);

	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	dataPtr2 = (LightBufferType*)mappedResource.pData;

	dataPtr2->ambientColor = ambientColor;
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->specularColor = specularColor;
	dataPtr2->specularPower = specularPower;

	deviceContext->Unmap(m_lightBuffer, 0);

	bufferNumber = 0;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	return true;
}

void Shader::RenderShader(ID3D11DeviceContext* deviceContext, INT indexCount)
{
	deviceContext->IASetInputLayout(m_layout);

	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}

bool Shader::ReadShaderFile(const WCHAR* vsFileName, const WCHAR* psFileName, HWND hwnd, ID3D10Blob** vertexShaderBuffer, ID3D10Blob** pixelShaderBuffer)
{
	HRESULT result;
	ID3D10Blob* errorMessage;

	errorMessage = nullptr;

	result = D3DCompileFromFile((WCHAR*)vsFileName, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, (WCHAR*)vsFileName);
		else
			MessageBox(hwnd, (WCHAR*)vsFileName, L"MissingShaderFile", MB_OK);

		return false;
	}

	result = D3DCompileFromFile((WCHAR*)psFileName, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, (WCHAR*)psFileName);
		else
			MessageBox(hwnd, (WCHAR*)psFileName, L"MissingShaderFile", MB_OK);

		return false;
	}

	return true;
}
