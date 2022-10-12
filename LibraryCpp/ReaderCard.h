#pragma once
#include "Subscriber.h"
#include <vector>
#include "Row.h"
#include "SubscriberRepository.h"
#include "RowRepository.h"

class ReaderCard
{
private:
	SubscriberRepository subscriberRepository;
	RowRepository rowRepository;
public:
	Subscriber subscriber;
	bool isBlocked;

	ReaderCard(Subscriber sub);

	void AddRow(Book book);

	std::vector<Row> Rows();

	void SwitchIsBlocked();
};