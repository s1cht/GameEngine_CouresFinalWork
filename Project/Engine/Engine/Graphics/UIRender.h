#pragma once
#include "pch.h"
#include "Engine/Core/Math/EngineMath.h"

#pragma comment(lib, "d2d1.lib")

using namespace D2D1;
using namespace DirectX;

class UIRender
{
public:
	UIRender();
	~UIRender();

public:
	bool Initialize(HWND, IDXGISurface*);
	void Shutdown();

	ID2D1RenderTarget* GetRenderTarget();

	void BeginScene();
	bool EndScene();

private:
	HWND m_hwnd;
	ID2D1RenderTarget* m_RenderTarget;

};

