#pragma once
#include <string>
#include <map>
#include <vector>

class JsonHelper
{
public:
	static std::vector<std::map<std::string, std::string>> Parse(std::string json);

	static std::string Serialize(std::map<std::string, std::string> obj);

	static std::string SerializeArray(std::vector< std::map<std::string, std::string>> arr);
};


