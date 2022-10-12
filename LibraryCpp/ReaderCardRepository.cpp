#include "ReaderCardRepository.h"

ReaderCardRepository::ReaderCardRepository()
{

}

ReaderCard ReaderCardRepository::Object(Subscriber subscriber)
{
	for (auto card : readerCards)
		if (card.subscriber.id == subscriber.id)
			return card;
}

std::vector<ReaderCard> ReaderCardRepository::Collection()
{
	return readerCards;
}

ReaderCard ReaderCardRepository::Create(std::string username)
{
	Subscriber s = subscriberRepository.Create(username);
	ReaderCard c = ReaderCard(s);
	readerCards.push_back(c);

	return c;
}

void ReaderCardRepository::Delete(Subscriber subscriber)
{
	for (int i = 0; i < readerCards.size(); i++)
		if (readerCards[i].subscriber.id == subscriber.id)
		{
			readerCards.erase(readerCards.begin() + i);
			subscriberRepository.Delete(subscriber);
			break;
		}
}

void ReaderCardRepository::AddRow(Subscriber subscriber, Book book)
{
	for (auto card : readerCards)
		if (card.subscriber.id == subscriber.id)
		{
			card.AddRow(book);
			break;
		}
}