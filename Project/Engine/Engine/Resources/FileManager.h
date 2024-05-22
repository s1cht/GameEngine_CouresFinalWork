#pragma once
#include "pch.h"
#include "SLPRSyntax.h"

class FileManager
{
public:
	FileManager();
	~FileManager();

#ifdef SL_ENGINE_EDITOR
	bool CallFileOpenScreen(HWND);
	bool CallFileSaveScreen(HWND);
	bool CallFileCreateScreen(HWND);
#else
	bool OpenFile(const wchar_t*);
#endif
	std::wstring GetFileName();

private:
	wchar_t m_fileName[MAX_PATH];

};

