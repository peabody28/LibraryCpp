#pragma once
#include "BookRepository.h"
#include "DateTime.h"
#include "Book.h"

class Row
{
private:
	int Id;
	BookRepository bookRepository;
public:
	int id;
	Book book;
	int readerCardId;
	DateTime dateFrom;
	DateTime dateTo;
	bool isReturned;

	Row();

	Row(Book _book, DateTime from, DateTime to);

	Row(int _id, int _bookId, int _readerCardId, DateTime from, DateTime to, bool _isRet);

	bool IsExpired();
};