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
	int id;
	Subscriber subscriber;
	bool isBlocked;

	ReaderCard();

	ReaderCard(Subscriber sub);

	void AddRow(Book book);

	std::vector<Row> Rows();

	void SwitchIsBlocked();
};