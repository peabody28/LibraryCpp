#pragma once
#include "ReaderCardRepository.h"
#include "WorkmanRepository.h"
#include "BookRepository.h"
#include "SubscriberRepository.h"
#include <string>

class Subscriber;
class Workman;
class Book;

class Library
{
public:
	BookRepository* bookRepository;
	ReaderCardRepository* readerCardRepository;
	SubscriberRepository* subscriberRepository;
	WorkmanRepository* workmanRepository;

	Library();

	void PrintBooks();
	
	void PrintSubscribers();

	void PrintBooks(int _subscriberId);

	void Debts();

	ReaderCard* AddSubcriber(std::string username);

	void SwitchSubscriberState(std::string name, Workman workman);

	void DeleteSubscriber(std::string name, Workman workman);

	void GiveBook(ReaderCard* readerCard, Book* book, Workman* workman);
};