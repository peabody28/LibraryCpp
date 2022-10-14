#pragma once
#include "Workman.h"
#include <vector>
#include <string>

class WorkmanRepository
{
private:
	static WorkmanRepository* obj;
	std::vector<Workman*> staff;

	void Build();

	WorkmanRepository();
public:

	static WorkmanRepository* GetInstance();

	Workman* Object(int _workmanId);
};