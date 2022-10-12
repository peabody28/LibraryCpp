#include "Row.h"
#include "Book.h"
#include "DateTime.h"
#include "DateTimeHelper.h"

Row::Row() {
	id = -1;
	isReturned = false;
	readerCardId = -1;
}

Row::Row(Book _book, DateTime from, DateTime to)
{
	id = Id;
	book = _book;
	dateFrom = from;
	dateTo = to;
	isReturned = false;
}

Row::Row(int _id, int _bookId, int _readerCardId, DateTime from, DateTime to, bool _isRet)
{
	id = _id;
	book = bookRepository.Object(_bookId);
	readerCardId = _readerCardId;
	dateFrom = from;
	dateTo = to;
	isReturned = _isRet;
}

bool Row::IsExpired()
{
	return DateTimeHelper::Compare(DateTimeHelper::Now(), dateTo) == 1;
}