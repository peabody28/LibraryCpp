#include "StringHelper.h"
#include <string>

std::string StringHelper::ToString(int num)
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

int StringHelper::ToInt(std::string s)
{
	int a = 0;
	auto current = --s.end();
	while (true)
	{
		a = a * 10 + *current - '0';
		if (current == s.begin())
			break;
		current--;
	}

	return a;
}

bool StringHelper::ToBool(std::string s)
{
	return s.compare("false");
}