#pragma once
#include "pch.h"
#include "IRender.h"

using namespace Microsoft::WRL;

class Render : public IRender
{
public:
	Render(UINT width, UINT height, std::wstring name);
	virtual void OnInit() override {}
	virtual void OnUpdate() override {}
	virtual void OnRender() override {}
	virtual void OnDestroy() override {}
private:
	ComPtr<IDXGIFactory> m_DXGIFactory;
	ComPtr<IDXGIAdapter> m_DXGIAdapter;
	ComPtr<ID3D12Device> m_D3D12Device;
	ComPtr<ID3D12CommandQueue> m_D3D12CommandQueue;
	ComPtr<ID3D12CommandAllocator> m_D3D12CommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> m_D3D12GraphicsCommandList;
};

