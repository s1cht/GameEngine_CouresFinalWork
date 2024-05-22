#pragma once
#include "pch.h"

const wchar_t SLPR_CHAR_EQUAL = L':';
const wchar_t SLPR_CHAR_BREAK = L';';
const wchar_t SLPR_CHAR_RESERVED_PREFIX = L'#';

const wchar_t SLPR_STR_RESERVED_WORD_NEW[] = L"newinst";
const wchar_t SLPR_STR_RESERVED_WORD_END[] = L"end";
const wchar_t SLPR_STR_PROJECT_NAME[] = L"Project_name";
const wchar_t SLPR_STR_TYPE[] = L"Type";
const wchar_t SLPR_STR_ID[] = L"ID";
const wchar_t SLPR_STR_NAME[] = L"Name";
const wchar_t SLPR_STR_PARENT[] = L"Parent";
const wchar_t SLPR_STR_POSITION[] = L"Position";
const wchar_t SLPR_STR_ROTATION[] = L"Rotation";
const wchar_t SLPR_STR_SIZE[] = L"Size";
const wchar_t SLPR_STR_COLOR[] = L"Color";
const wchar_t SLPR_STR_MESH[] = L"Mesh";
const wchar_t SLPR_STR_TEXTURE[] = L"Texture";
const wchar_t SLPR_STR_NOTHING[] = L"null";

namespace SLPR
{
	extern void BreakLine(std::wofstream&);
	extern void AddValueLine(std::wofstream&, const wchar_t*, const wchar_t*);
	extern void AddValueLine(std::wofstream&, const wchar_t*, std::wstring);
	extern void AddValueLine(std::wofstream&, const wchar_t*, std::wstring);
	extern void AddValueLine(std::wofstream&, const wchar_t*, INT);
	extern void AddValueLine(std::wofstream&, const wchar_t*, FLOAT);
	extern void AddValueLine(std::wofstream&, const wchar_t*, FLOAT, FLOAT);
	extern void AddValueLine(std::wofstream&, const wchar_t*, FLOAT, FLOAT, FLOAT);
	extern void AddValueLine(std::wofstream&, const wchar_t*, FLOAT, FLOAT, FLOAT, FLOAT);
	extern void AddReservedLine(std::wofstream&, const wchar_t*);
	extern void AddEmptyLine(std::wofstream&);
	extern void WaitUntilEqual(std::wifstream&);
	extern void WaitUntilReserved(std::wifstream&, bool&);
}