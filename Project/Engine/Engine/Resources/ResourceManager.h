#pragma once

#include "pch.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "Engine/Resources/ResourceList.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	bool Initialize(INT, INT);
	void Shutdown();

	//bool LoadAllResourceExcludingObjects(ID3D11Device*, ID3D11DeviceContext*, HWND, INT*, INT*, size_t, size_t);

	bool LoadObject(ID3D11Device*, ID3D11DeviceContext*, std::string, const WCHAR*);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, std::string, const WCHAR*);
	bool LoadShader(ID3D11Device*, HWND, std::string, const WCHAR*, const WCHAR*);

	Texture* GetTexture(std::string);
	Mesh* GetMesh(std::string);
	Shader* GetShader(std::string);
	
	Texture** GetTextures(UINT&);
	Mesh** GetMeshes(UINT&);
	Shader** GetShaders(UINT&);

	std::wstring GetLoadedStringByID(INT);

private:
	std::unordered_map<INT, std::wstring> m_LoadedStrings;

	std::vector<Texture*>  m_Textures;
	std::vector<Mesh*> m_Meshes;
	std::vector<Shader*> m_Shaders;

};

