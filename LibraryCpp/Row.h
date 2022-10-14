#pragma once
#include "BookRepository.h"
#include "DateTime.h"
#include "Book.h"
#include "ReaderCardRepository.h"
#include "ReaderCard.h"

class Row
{
private:
	BookRepository* bookRepository;
	ReaderCardRepository* readerCardRepository;
public:
	int id;
	Book book;
	ReaderCard readerCard;
	DateTime dateFrom;
	DateTime dateTo;
	bool isReturned;

	Row();

	Row(Book _book, DateTime from, DateTime to);

	Row(int _id, int _bookId, int _readerCardId, DateTime from, DateTime to, bool _isRet);

	bool IsExpired();
};