#pragma once
#include <string>
#include "WorkmanType.h"

class Workman
{
private:
	std::string name;
public:
	WorkmanType type;

	friend class WorkmanRepository;
};

