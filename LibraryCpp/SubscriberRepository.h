#pragma once
#include "Subscriber.h"
#include <vector>
#include <string>

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