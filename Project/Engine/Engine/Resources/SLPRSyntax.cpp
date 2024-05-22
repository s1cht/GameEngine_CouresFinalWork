#include "pch.h"
#include "SLPRSyntax.h"

void SLPR::BreakLine(std::wofstream& file)
{
}

void SLPR::AddValueLine(std::wofstream& file, const wchar_t* type, const wchar_t* value)
{
	file << type << SLPR_CHAR_EQUAL << value << SLPR_CHAR_BREAK << std::endl;
}

void SLPR::AddValueLine(std::wofstream& file, const wchar_t* type, std::wstring value)
{
	file << type << SLPR_CHAR_EQUAL << value << SLPR_CHAR_BREAK << std::endl;
}

void SLPR::AddValueLine(std::wofstream& file, const wchar_t* type, INT value)
{
	file << type << SLPR_CHAR_EQUAL << value << SLPR_CHAR_BREAK << std::endl;
}

void SLPR::AddValueLine(std::wofstream& file, const wchar_t* type, FLOAT value)
{
	file << type << SLPR_CHAR_EQUAL << value << SLPR_CHAR_BREAK << std::endl;
}

void SLPR::AddValueLine(std::wofstream& file, const wchar_t* type, FLOAT a, FLOAT b)
{
	file << type << SLPR_CHAR_EQUAL << a << " " << b << SLPR_CHAR_BREAK << std::endl;
}

void SLPR::AddValueLine(std::wofstream& file, const wchar_t* type, FLOAT a, FLOAT b, FLOAT c)
{
	file << type << SLPR_CHAR_EQUAL << a << " " << b << " " << c << SLPR_CHAR_BREAK << std::endl;
}

void SLPR::AddValueLine(std::wofstream& file, const wchar_t* type, FLOAT a, FLOAT b, FLOAT c, FLOAT d)
{
	file << type << SLPR_CHAR_EQUAL << a << " " << b << " " << c << " " << d << SLPR_CHAR_BREAK << std::endl;
}

void SLPR::AddReservedLine(std::wofstream& file, const wchar_t* word)
{
	file << SLPR_CHAR_RESERVED_PREFIX << word << std::endl;
}

void SLPR::AddEmptyLine(std::wofstream& file)
{
	file << std::endl;
}

void SLPR::WaitUntilEqual(std::wifstream& file)
{
	wchar_t input;
	std::wstring buff;
	file.get(input);
	while (input != SLPR_CHAR_EQUAL)
		file.get(input);
}

void SLPR::WaitUntilReserved(std::wifstream& file, bool& value)
{
	wchar_t input;
	std::wstring buff;
	file.get(input);
	while (input != SLPR_CHAR_RESERVED_PREFIX)
		file.get(input);
	getline(file, buff);

	if (buff == SLPR_STR_RESERVED_WORD_END)
		value = true;
	else if (buff == SLPR_STR_RESERVED_WORD_NEW)
		return;
}
