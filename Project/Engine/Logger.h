#pragma once
#include "pch.h"

class Logger
{
private:
	std::ofstream file;
public:
	Logger();
	~Logger();
public:
	void Log(std::string Object, std::string Message);
};

