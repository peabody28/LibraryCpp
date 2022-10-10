#pragma once
#include "DateTime.h"
#include "Book.h"
#include "BookRepository.h"

class Row
{
private:
	BookRepository bookRepository;
public:
	int number;
	int bookId;
	DateTime dateFrom;
	DateTime dateTo;
	bool isReturned;
	
	Book GetBook()
	{
		bookRepository.Object(bookId);
	}
};