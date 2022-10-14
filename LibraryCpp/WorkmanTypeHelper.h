#pragma once
#include "WorkmanType.h"
#include <string>

class WorkmanTypeHelper
{
public:
	static WorkmanType Parse(std::string type);
};