#include "pch.h"
#include "Instance.h"

InstanceService::InstanceService()
{
	m_device = nullptr;
	m_deviceContext = nullptr;
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