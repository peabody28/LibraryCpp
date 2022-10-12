#pragma once
#include <vector>
#include "Subscriber.h"
#include "ReaderCard.h"

class ReaderCardRepository
{
private:
	SubscriberRepository subscriberRepository;
	std::vector<ReaderCard> readerCards; 
public:

	ReaderCardRepository();

	ReaderCard Object(Subscriber subscriber);

	std::vector<ReaderCard> Collection();

	ReaderCard Create(std::string username);

	void Delete(Subscriber subscriber);

	void AddRow(Subscriber subscriber, Book book);
};