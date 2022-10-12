#pragma once
#include "DateTime.h"
#include "Book.h"
#include "BookRepository.h"

class Row
{
private:
	static int Id;
	BookRepository bookRepository;
public:
	int id;
	Book book;
	DateTime dateFrom;
	DateTime dateTo;
	bool isReturned;

	Row(Book _book, DateTime from, DateTime to);

	Row(int _id, int _bookId, DateTime from, DateTime to, bool _isRet);

	bool IsExpired();
};