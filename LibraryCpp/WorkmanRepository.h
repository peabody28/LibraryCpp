#pragma once
#include <vector>
#include "Workman.h"

class WorkmanRepository
{
private:
	std::vector<Workman> staff;
public:
	Workman ObjectByName(std::string name)
	{
		for (auto man : staff)
			if (!man.name.compare(name))
				return man;
	}
};