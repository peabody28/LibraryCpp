#include <vector>
#include "Workman.h"
#include "ReaderCard.h"
#include "DateTime.h"
#include <iostream>
#include "PermissionDeniedException.h"
#include "ReaderCardRepository.h"

std::ostream& operator << (std::ostream& os, const DateTime& d)
{
	return os << d.day << " " << d.month << " " << d.year;
}

std::ostream& operator << (std::ostream& os, const Subscriber& s)
{
	return os << s.id << " " << s.name;
}

std::ostream& operator << (std::ostream& os, const Book& s)
{
	return os << s.id << " " << s.author << " " << s.title;
}

class Library
{
private:
	ReaderCardRepository readerCardRepository;
	SubscriberRepository subscriberRepository;
	std::vector<Workman> staff;
	BookRepository bookRepository;
public:

	void PrintBooks()
	{
		for (auto book : bookRepository.Collection())
			std::cout << book << std::endl;
	}

	void PrintSubscribers()
	{
		for (auto card : readerCardRepository.Collection())
			std::cout << card.subscriber << std::endl;
	}

	void PrintBooks(int _subscriberId)
	{
		for (auto card : readerCardRepository.Collection())
			if (card.subscriber.id == _subscriberId)
			{
				for (auto row : card.Rows())
					std::cout << row.book << " " << "dateFrom: " << row.dateFrom << " DateTo: " << row.dateTo << std::endl;
				break;
			}	
	}

	void Debts()
	{
		for (auto card : readerCardRepository.Collection())
			for (auto row : card.Rows())
			{
				if(row.IsExpired())
					std::cout << card.subscriber << " " << row.book << " " << "dateFrom: " << row.dateFrom << " DateTo: " << row.dateTo << std::endl;
			}		
	}

	void AddSubcriber(std::string username)
	{
		readerCardRepository.Create(username);
	}

	void SwitchSubscriberState(std::string name, Workman workman)
	{
		if (workman.type != Manager && workman.type != Director)
			throw PermissionDeniedException();

		for (auto card : readerCardRepository.Collection())
			if (!card.subscriber.name.compare(name))
			{
				card.SwitchIsBlocked();
				break;
			}
	}

	void DeleteSubscriber(std::string name, Workman workman)
	{
		if (workman.type != Director)
			throw PermissionDeniedException();

		for (auto card : readerCardRepository.Collection())
			if (!card.subscriber.name.compare(name))
			{
				readerCardRepository.Delete(card.subscriber);
				break;
			}
	}

	void GiveBook(int subscriberId, Book book, Workman workman)
	{
		Subscriber subscriber = subscriberRepository.Object(subscriberId);
		readerCardRepository.AddRow(subscriber, book);
	}
};