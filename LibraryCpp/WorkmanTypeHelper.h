#pragma once
#include <string>

enum WorkmanType;

class WorkmanTypeHelper
{
public:
	static WorkmanType Parse(std::string type);
};