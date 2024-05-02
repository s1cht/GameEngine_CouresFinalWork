#include "pch.h"
#include "ResourceManager.h"

bool ResourceManager::LoadObject(char* fileName, INT& indexCount, INT& vertexCount, ModelType* objectInformation)
{
    std::ifstream file;
    char input;
    int i;

    file.open(fileName);

    if (file.fail())
        return false;

    file.get(input);
    while (input != ':')
        file.get(input);

    file >> vertexCount;

    indexCount = vertexCount;

    objectInformation = new ModelType[vertexCount];

    if (!objectInformation)
        return false;

    file.get(input);
    while (input != ':')
        file.get(input);
    file.get(input);
    file.get(input);

    for (i = 0; i < vertexCount; i++)
    {
        file >> objectInformation[i].x >> objectInformation[i].y >> objectInformation[i].z;
        file >> objectInformation[i].tu >> objectInformation[i].tv;
        file >> objectInformation[i].nx >> objectInformation[i].ny >> objectInformation[i].nz;
    }

    file.close();

    return true;
}
