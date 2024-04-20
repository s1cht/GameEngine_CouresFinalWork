#include "pch.h"
#include "IRender.h"

IRender::IRender(UINT width, UINT height, std::wstring name)
	: m_width(width), m_height(height), m_title(name) {}
