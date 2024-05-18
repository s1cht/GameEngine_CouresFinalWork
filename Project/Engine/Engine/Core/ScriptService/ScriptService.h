#pragma once
#include "pch.h"

using namespace LuaCpp;
using namespace LuaCpp::Registry;
using namespace LuaCpp::Engine;

class ScriptService
{
public:
	ScriptService();
	~ScriptService();

public:
	bool Initialize();
	void Shutdown();

private:
	LuaContext lua;

	std::map<std::string, std::wstring> scripts;

};

