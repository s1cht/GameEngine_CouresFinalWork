#include "pch.h"
#include "Instance.h"

InstanceService::InstanceService()
{
	m_device = nullptr;
	m_deviceContext = nullptr;
}

std::wstring InstanceService::ConvertClassNameToWString(ClassName name)
{
	switch (name)
	{
	case CAMERA: 
	{
		return L"Camera";
		break;
	}
	case WORLD:
	{
		return L"World";
		break;
	}
	case PART:
	{
		return L"Part";
		break;
	}
	case SUNLIGHT:
	{
		return L"SunLight";
		break;
	}
	default:
	{
		return L"unknown";
		break;
	}
	}
}

void InstanceService::SetParent(INSTANCE parent, INSTANCE child)
{
	if (child->GetParent())
		child->GetParent()->DeleteChild(child->GetID());
	parent->AddChild(child);
	child->SetParent(parent);
}

void InstanceService::SetDevice(ID3D11Device* device)
{
	m_device = device;
}

void InstanceService::SetDeviceContext(ID3D11DeviceContext* deviceContext)
{
	m_deviceContext = deviceContext;
}

void InstanceService::Shutdown()
{
	m_device = nullptr;
	m_deviceContext = nullptr;
}

INT InstanceService::GetInstanceCount()
{
	return m_instanceCount;
}
