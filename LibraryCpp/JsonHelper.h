#pragma once
#include <string>
#include <map>
#include <vector>

class JsonHelper
{
public:
	static std::vector<std::map<std::string, std::string>> Parse(std::string json);
};


