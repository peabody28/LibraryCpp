#pragma once
#include <vector>
#include "Subscriber.h"

// subscribers.txt

class SubscriberRepository
{
private:
	std::vector<Subscriber> subscribers;
public:
	Subscriber Object(int id)
	{
		for (auto sub : subscribers)
			if (sub.id == id)
				return sub;
	}

	Subscriber Create(std::string name)
	{
		Subscriber s = Subscriber(name);
		subscribers.push_back(s);
		return s;
	}

	void Delete(Subscriber subscriber)
	{
		for (int i = 0; i < subscribers.size(); i++)
			if (subscribers[i].id == subscriber.id)
			{
				subscribers.erase(subscribers.begin() + i);
				return;
			}
	}
};