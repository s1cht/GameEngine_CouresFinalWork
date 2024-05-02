#include "pch.h"
#include "Instance.h"

Instance::Instance()
{
}

Instance::~Instance()
{
}

void Instance::Destroy()
{
}

bool Instance::SetParent(Instance* parent)
{
    if (!parent)
        return false;
    
    Parent = parent;
    return true;
}
