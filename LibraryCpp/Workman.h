#pragma once
#include <string>
#include "WorkmanType.h"

class Workman
{
public:
	int id;
	WorkmanType type;

	Workman();

	Workman(WorkmanType w);
};

