#pragma once
#include "Workman.h"
#include <vector>
#include <string>

class WorkmanRepository
{
private:
	std::vector<Workman> staff;

	void Build();
public:

	WorkmanRepository();

	Workman ObjectByName(std::string name);
};