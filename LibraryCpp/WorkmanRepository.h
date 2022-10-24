#pragma once
#include "Workman.h"
#include "IRepository.h"

class WorkmanRepository : public IRepository<Workman>
{
private:
	static WorkmanRepository* obj;

	WorkmanRepository();

protected:
	std::vector<Workman*> Build() override;

	std::map<std::string, std::string> Serialize(Workman* obj) override;

public:

	static WorkmanRepository* GetInstance();

	Workman* Object(WorkmanType w);

	Workman* Add(int id, WorkmanType w);
};