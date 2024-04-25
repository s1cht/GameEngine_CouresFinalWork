#include "pch.h"
#include "Logger.h"

Logger::Logger(): file("Logs/" + std::to_string(time(NULL)) + ".txt") {}

Logger::~Logger() 
{
	file.close();
}

void Logger::Log(std::string Object, std::string Message)
{
	file << Object << ": " << Message << ";" << std::endl;
}
