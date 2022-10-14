#pragma once
#include <vector>
#include "SubscriberRepository.h"

class ReaderCard;
class Subscriber;

class ReaderCardRepository
{
private:
	SubscriberRepository* subscriberRepository;
	std::vector<ReaderCard*> readerCards;

	void Build();

	ReaderCardRepository();

	static ReaderCardRepository* obj;

public:

	static ReaderCardRepository* GetInstance();

	ReaderCard Object(Subscriber subscriber);

	ReaderCard Object(int subscriberId);

	std::vector<ReaderCard*> Collection();

	ReaderCard Create(std::string username);

	void Delete(Subscriber subscriber);

	void AddRow(Subscriber subscriber, Book book);
};