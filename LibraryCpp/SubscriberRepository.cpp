#include "SubscriberRepository.h"

Subscriber SubscriberRepository::Object(int id)
{
	for (auto sub : subscribers)
		if (sub.id == id)
			return sub;
}

Subscriber SubscriberRepository::Create(std::string name)
{
	Subscriber s = Subscriber(name);
	subscribers.push_back(s);
	return s;
}

void SubscriberRepository::Delete(Subscriber subscriber)
{
	for (int i = 0; i < subscribers.size(); i++)
		if (subscribers[i].id == subscriber.id)
		{
			subscribers.erase(subscribers.begin() + i);
			return;
		}
}