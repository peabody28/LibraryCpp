#pragma once
#include <vector>
#include "SubscriberRepository.h"
#include "RowRepository.h"
#include "Subscriber.h"

class Row;

class ReaderCard
{
private:
	SubscriberRepository* subscriberRepository;
	RowRepository* rowRepository;
public:
	int id;
	Subscriber subscriber;
	bool isBlocked;

	ReaderCard();

	ReaderCard(Subscriber sub);

	ReaderCard(int _id, int _subscriberId, bool _isBlocked);

	void AddRow(Book book);

	std::vector<Row*> Rows();

	void SwitchIsBlocked();
};