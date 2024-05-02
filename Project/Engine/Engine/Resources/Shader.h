#pragma once
#include "pch.h"

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

	struct LightBufferType
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		FLOAT padding;
	};

public:
	Shader();
	Shader(const Shader&);
	~Shader();
	
	bool Initialize(ID3D11Device*, HWND, ID3D10Blob**, ID3D10Blob**);
	void Shutdown();

	bool Render(ID3D11DeviceContext*, INT, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);

	void SetName(string);

	string GetName();

private:
	bool InitializeShader(ID3D11Device*, HWND, ID3D10Blob**, ID3D10Blob**);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);
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

private:
	friend class ResourceManager;
};

