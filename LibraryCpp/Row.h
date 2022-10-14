#pragma once
#include "BookRepository.h"
#include "ReaderCardRepository.h"
#include "DateTime.h"

class ReaderCard;
class Book;

class Row
{
private:
	BookRepository* bookRepository;
	ReaderCardRepository* readerCardRepository;

public:
	int id;
	int bookId;
	int readerCardId;
	DateTime dateFrom;
	DateTime dateTo;
	bool isReturned;

	Row();

	Row(Book* bookId, ReaderCard* readerCard, DateTime f, DateTime t);

	Row(int _id, int _bookId, int _readerCardId, DateTime from, DateTime to, bool _isRet);

	ReaderCard* _ReaderCard();

	Book* _Book();

	bool IsExpired();
};