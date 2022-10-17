#pragma once
#include <vector>
#include <string>

class Subscriber;

class SubscriberRepository
{
private:
	static SubscriberRepository* obj;
	std::vector<Subscriber*> subscribers;

	void Build();

	SubscriberRepository();

public:
	void static UpdateData();

	static SubscriberRepository* GetInstance();

	Subscriber* Object(int id);

	Subscriber* Create(std::string name);

	void Delete(Subscriber* subscriber);
};