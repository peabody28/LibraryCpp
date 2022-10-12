#include "WorkmanRepository.h"
#include "Workman.h"
#include <string>
#include <vector>

void WorkmanRepository::Build()
{

}

WorkmanRepository::WorkmanRepository() {}

Workman WorkmanRepository::ObjectByName(std::string name)
{
	for (auto man : staff)
		if (!man.name.compare(name))
			return man;
}
