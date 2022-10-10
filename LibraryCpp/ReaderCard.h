#pragma once
#include "Subscriber.h"
#include <vector>
#include "Row.h"
#include "SubscriberRepository.h"

class ReaderCard
{
private:
	SubscriberRepository subscriberRepository;
public:
	std::vector<Row> rows;
	int subscriberId;
	bool isBlocked;

	Subscriber Subscriber()
	{
		return subscriberRepository.Object(subscriberId);
	}
};