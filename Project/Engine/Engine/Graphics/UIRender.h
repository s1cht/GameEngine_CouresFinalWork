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
	bool Initialize(HWND);
	void Shutdown();

	bool Frame();

private:
	bool Render();

private:
	HWND m_hwnd;
	ID2D1HwndRenderTarget* m_RenderTarget;

};

