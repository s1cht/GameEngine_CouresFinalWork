#pragma once
#include "pch.h"
#include "Engine/Core/Math/EngineMath.h"

class Texture
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
	Texture();
	~Texture();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, UCHAR**, Vector2);
	void Shutdown();

	void SetName(std::string);

	std::string GetName();
	INT GetWidth();
	INT GetHeight();
	ID3D11ShaderResourceView* GetTexture();

private:
	static bool ReadTargaFile(const char*, UCHAR**, Vector2&);

private:
	std::string m_name;

	UCHAR* m_TargaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
	Vector2 m_size;

private:
	friend class ResourceManager;

};

