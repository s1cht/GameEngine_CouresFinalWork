#pragma once
#include "pch.h"
#include "Engine/Core/Math/EngineMath.h"

class TextureClass
{
private:
	struct TargaHeader
	{
		UCHAR data1[12];
		USHORT width;
		USHORT height;
		UCHAR bpp;
		UCHAR data2;
	};

public:
	TextureClass();
	~TextureClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	INT GetWidth();
	INT GetHeight();

private:
	bool LoadTarga32B(char*);

private:
	UCHAR* m_TargaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
	Vector2 m_size;
};

