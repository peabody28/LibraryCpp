#pragma once
#include "DateTime.h"
#include "Book.h"
#include "BookRepository.h"
#include "DateTimeHelper.h"

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

	Row(Book _book, DateTime from, DateTime to)
	{
		Id = 1;
		id = Id;
		book = _book;
		dateFrom = from;
		dateTo = to;
		isReturned = false;
	}

	bool IsExpired()
	{
		return DateTimeHelper::Compare(DateTimeHelper::Now(), dateTo) == 1;
	}
};