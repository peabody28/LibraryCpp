#pragma once
#include "Subscriber.h"
#include <vector>
#include <ctime>
#include "Row.h"
#include "SubscriberRepository.h"
#include "DateTimeHelper.h"
#include "RowRepository.h"

class ReaderCard
{
private:
	SubscriberRepository subscriberRepository;
	RowRepository rowRepository;
public:
	Subscriber subscriber;
	bool isBlocked;

	ReaderCard(Subscriber sub)
	{
		subscriber = sub;
		isBlocked = false;
	}

	void AddRow(Book book)
	{
		DateTime now = DateTimeHelper::Now();
		Row r = Row(book, now, DateTimeHelper::AddMonth(now));
		rowRepository.Create(r);
	}

	std::vector<Row> Rows()
	{
		return rowRepository.Collection();
	}

	void SwitchIsBlocked()
	{
		isBlocked = !isBlocked;
	}
};