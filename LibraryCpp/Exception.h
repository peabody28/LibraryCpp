#pragma once
#include <string>

class Exception
{
public:
	std::string msg;

	Exception(std::string _msg);
};