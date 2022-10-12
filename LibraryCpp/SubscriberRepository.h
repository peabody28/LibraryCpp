#pragma once
#include <vector>
#include "Subscriber.h"

class SubscriberRepository
{
private:
	std::vector<Subscriber> subscribers;

	void Build();
public:
	SubscriberRepository();

	Subscriber Object(int id);

	Subscriber Create(std::string name);

	void Delete(Subscriber subscriber);
};