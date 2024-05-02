#pragma once

#include "pch.h"

const std::wstring ASSETS = L"../Engine/Assets/";


enum TexturesValues {
	STONE01,
	DIRT01
};

const std::wstring Textures[] = {
	ASSETS + L"Textures/" + L"stone01.tga",
	ASSETS + L"Textures/" + L"dirt01.tga",
};

enum MeshesValues {
	DEFAULT_CUBE
};

const std::wstring Meshes[] = {
	ASSETS + L"Meshes/" + L"cube.txt",
};

enum ShadersValues {
	PIXEL_SHADER,
	VERTEX_SHADER
};

const std::wstring Shaders[] = {
	ASSETS + L"Shaders/" + L"PixelLightShader.hlsl",
	ASSETS + L"Shaders/" + L"VertexLightShader.hlsl",
};