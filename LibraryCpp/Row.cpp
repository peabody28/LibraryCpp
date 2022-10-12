#include "Row.h"
#include "DateTimeHelper.h"

int Row::Id = 0;

Row::Row(Book _book, DateTime from, DateTime to)
{
	id = Id;
	book = _book;
	dateFrom = from;
	dateTo = to;
	isReturned = false;
}

Row::Row(int _id, int _bookId, DateTime from, DateTime to, bool _isRet)
{
	id = _id;
	book = bookRepository.Object(_bookId);
	dateFrom = from;
	dateTo = to;
	isReturned = _isRet;
}

bool Row::IsExpired()
{
	return DateTimeHelper::Compare(DateTimeHelper::Now(), dateTo) == 1;
}