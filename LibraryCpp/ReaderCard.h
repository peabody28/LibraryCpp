#pragma once
#include <vector>
#include "SubscriberRepository.h"
#include "RowRepository.h"

class Row;
class Book;
class Subscriber;

class ReaderCard
{
private:
	SubscriberRepository* subscriberRepository;
	RowRepository* rowRepository;
public:
	int id;
	int subscriberId;
	bool isBlocked;
	bool isDeleted;

	ReaderCard();

	ReaderCard(Subscriber* sub);

	ReaderCard(int _id, int _subscriberId, bool _isBlocked, bool _isDeleted);

	Subscriber* _Subscriber();

	void AddRow(Book* book);

	std::vector<Row*> Rows();

	void SwitchIsBlocked();
};