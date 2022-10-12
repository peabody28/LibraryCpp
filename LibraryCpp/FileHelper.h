#pragma once
#include <fstream>
#include <string>

class FileHelper
{
public:
	static std::string Read(std::ifstream* f);
};