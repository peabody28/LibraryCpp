#pragma once
#include <string>

enum WorkmanType;

class WorkmanTypeHelper
{
public:
	static WorkmanType Parse(std::string type);

	static std::string ToString(WorkmanType w);
};