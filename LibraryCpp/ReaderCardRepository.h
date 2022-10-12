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

	ReaderCardRepository()
	{
	
	}

	ReaderCard Object(Subscriber subscriber)
	{
		for (auto card : readerCards)
			if (card.subscriber.id == subscriber.id)
				return card;
	}

	std::vector<ReaderCard> Collection()
	{
		return readerCards;
	}

	ReaderCard Create(std::string username)
	{
		Subscriber s = subscriberRepository.Create(username);
		ReaderCard c = ReaderCard(s);
		readerCards.push_back(c);

		return c;
	}

	void Delete(Subscriber subscriber)
	{
		for(int i = 0; i < readerCards.size(); i++)
			if (readerCards[i].subscriber.id == subscriber.id)
			{
				readerCards.erase(readerCards.begin() + i);
				subscriberRepository.Delete(subscriber);
				break;
			}
	}

	void AddRow(Subscriber subscriber, Book book)
	{
		for (auto card : readerCards)
			if (card.subscriber.id == subscriber.id)
			{
				card.AddRow(book);
				break;
			}
	}
};