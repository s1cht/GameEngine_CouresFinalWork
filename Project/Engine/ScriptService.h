#pragma once
#include "pch.h"

class ScriptService
{
public:
	ScriptService();
	~ScriptService();

public:
	bool Initialize();
	void Shutdown();

private:
	std::map<std::string, std::ifstream> scripts;

};

