#pragma once
#include <string>

static class StringHelper
{
public:
	static std::string ToString(int num)
	{
		char* n = new char[9];
		n += 8;
		*n = '\0';
		n--;

		while (num != 0)
		{
			*n-- = num % 10;
			num /= 10;
		}

		return std::string(n);
	}
};