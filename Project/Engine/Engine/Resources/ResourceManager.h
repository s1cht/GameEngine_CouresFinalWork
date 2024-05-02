#pragma once

#include "pch.h"

struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
};
struct ModelType
{
	FLOAT x, y, z;
	FLOAT tu, tv;
	FLOAT nx, ny, nz;
};

class ResourceManager
{
public:
	static bool LoadObject(char* fileName, INT& indexCount, INT& vertexCount, ModelType* objectInformation);

private:


};

