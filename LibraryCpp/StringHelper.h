#pragma once
#include <string>

static class StringHelper
{
public:
	static std::string ToString(int num);

	static int ToInt(std::string s);

	static bool ToBool(std::string s);
};