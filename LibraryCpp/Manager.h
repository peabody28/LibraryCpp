#pragma once
#include "Workman.h"

namespace staff
{
	class Manager : public Workman
	{
	public:
		Manager();

		Manager(WorkmanType w);
	};
}
