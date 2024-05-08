#pragma once
#include "pch.h"
#include "Engine/Core/Instance/Instances/Part.h"
#include "Engine/Core/Instance/Instances/Light.h"

using namespace DirectX;
using namespace std;

class Shader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		FLOAT padding;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		FLOAT specularPower;
		XMFLOAT4 specularColor;
		XMFLOAT4 partColor;
	};

public:
	Shader();
	Shader(const Shader&);
	~Shader();
	
	bool Initialize(ID3D11Device*, HWND, ID3D10Blob**, ID3D10Blob**);
	void Shutdown();

	bool Render(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, Part*, Light*, XMFLOAT3);

	void SetName(string);

	string GetName();

private:
	bool InitializeShader(ID3D11Device*, HWND, ID3D10Blob**, ID3D10Blob**);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, Part*, Light*, XMFLOAT3);
	void RenderShader(ID3D11DeviceContext*, INT);

	static bool ReadShaderFile(const WCHAR*, const WCHAR*, HWND, ID3D10Blob**, ID3D10Blob**);
	static void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

private:
	string m_name;

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_cameraBuffer;

private:
	friend class ResourceManager;
};

