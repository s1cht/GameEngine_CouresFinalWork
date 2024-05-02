#pragma once

#include "pch.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "FilesToLoad.h"


class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	bool Initialize();
	void Shutdown();

	bool LoadObject(ID3D11Device*, ID3D11DeviceContext*, Texture**, std::string, const WCHAR*);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, std::string, const WCHAR*);
	bool LoadShader(ID3D11Device*, HWND, std::string, const WCHAR*, const WCHAR*);

	Texture* GetTexture(std::string);
	Mesh* GetMesh(std::string);
	Shader* GetShader(std::string);
	
	Texture** GetTextures(UINT&);
	Mesh** GetMeshes(UINT&);
	Shader** GetShaders(UINT&);

private:

private:
	std::vector<Texture*>  m_Textures;
	std::vector<Mesh*> m_Meshes;
	std::vector<Shader*> m_Shaders;

};

