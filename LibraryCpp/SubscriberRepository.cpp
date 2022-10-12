#include "SubscriberRepository.h"
#include "FileHelper.h"
#include "JsonHelper.h"
#include "StringHelper.h"


void SubscriberRepository::Build()
{
	std::ifstream f("subscribers.json");

	std::string json = FileHelper::Read(&f);

	auto collection = JsonHelper::Parse(json);

	for (auto row : collection)
	{
		Subscriber tmp = Subscriber(StringHelper::ToInt(row["id"]), row["name"]);
		subscribers.push_back(tmp);
	}
}

SubscriberRepository::SubscriberRepository()
{
	Build();
}

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