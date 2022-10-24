#pragma once
#include <vector>
#include <string>
#include "IRepository.h"

class Subscriber;

class SubscriberRepository : public IRepository<Subscriber>
{
private:
	static SubscriberRepository* obj;

	SubscriberRepository();

protected:
	std::vector<Subscriber*> Build() override;

	std::map<std::string, std::string> Serialize(Subscriber* obj)  override;

public:
	static SubscriberRepository* GetInstance();

	Subscriber* Object(int id);

	Subscriber* Object(std::string name);

	Subscriber* Create(std::string name);
};