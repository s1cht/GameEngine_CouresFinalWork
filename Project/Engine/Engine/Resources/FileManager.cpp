#include "pch.h"
#include "FileManager.h"

FileManager::FileManager()
{
	m_fileName[0] = '\0';
}

FileManager::~FileManager()
{
}

#ifdef SL_ENGINE_EDITOR
bool FileManager::CallFileOpenScreen(HWND hwnd)
{
	OPENFILENAME ofn;

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = m_fileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(m_fileName);
	ofn.lpstrFilter = L"SL Engine Project Files (*.slpr)\0*.slpr";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"~\\Documents\\";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	BOOL result = GetOpenFileName(&ofn);

	if (result)
	{
		return true;
	}

	return false;
}

bool FileManager::CallFileSaveScreen(HWND hwnd)
{
	OPENFILENAME ofn;

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = m_fileName;
	ofn.lpstrFilter = L"SL Engine Project Files (*.slpr)\0*.slpr";
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(m_fileName);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = sizeof(m_fileName);
	ofn.lpstrInitialDir = L"~\\Documents\\";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrTitle = L"Save project as...";

	// Display the Open dialog box. 

	if (GetSaveFileName(&ofn) == TRUE)
	{
		wcscat_s(m_fileName, L".slpr");

		return true;
	}

	return false;
}

bool FileManager::CallFileCreateScreen(HWND hwnd)
{
	OPENFILENAME ofn;

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = m_fileName;
	ofn.lpstrFilter = L"SL Engine Project Files (*.slpr)\0*.slpr";
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(m_fileName);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = sizeof(m_fileName);
	ofn.lpstrInitialDir = L"~\\Documents\\";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrTitle = L"Create project...";

	// Display the Open dialog box. 

	if (GetSaveFileName(&ofn) == TRUE)
	{
		wcscat_s(m_fileName, L".slpr");

		return true;
	}

	return false;
}



#else
bool Project::OpenFile(const wchar_t*)
{
	return true;
}
#endif

std::wstring FileManager::GetFileName()
{
	return std::wstring(m_fileName);
}
