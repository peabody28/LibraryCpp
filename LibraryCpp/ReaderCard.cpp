#include "ReaderCard.h"
#include "DateTimeHelper.h"

ReaderCard::ReaderCard() {}

ReaderCard::ReaderCard(Subscriber sub)
{
	subscriber = sub;
	isBlocked = false;
}

void ReaderCard::AddRow(Book book)
{
	DateTime now = DateTimeHelper::Now();
	Row r = Row(book, now, DateTimeHelper::AddMonth(now));
	rowRepository.Create(r);
}

std::vector<Row> ReaderCard::Rows()
{
	return rowRepository.Collection();
}

void ReaderCard::SwitchIsBlocked()
{
	isBlocked = !isBlocked;
}