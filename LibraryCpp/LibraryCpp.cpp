#include <iostream>
#include "Book.h"
#include "Row.h"
#include "ReaderCard.h"
#include "Subscriber.h"
#include "BookRepository.h"
#include "RowRepository.h"
#include "SubscriberRepository.h"
#include "ReaderCardRepository.h"
#include "StringHelper.h"
#include "FileHelper.h"
#include "JsonHelper.h"
#include <fstream>
#include <string>
#include <vector>
#include "DateTime.h"
#include "DateTimeHelper.h"
#include "PermissionDeniedException.h"
#include "Exception.h"
#define PERMISSION_DENIED_FOR_THIS_ACTION "PERMISSION_DENIED_FOR_THIS_ACTION"

// ReaderCard
ReaderCard::ReaderCard() {}

ReaderCard::ReaderCard(Subscriber sub)
{
	subscriber = sub;
	isBlocked = false;
}

ReaderCard::ReaderCard(int _id, int _subscriberId, bool _isBlocked)
{
	subscriberRepository = SubscriberRepository::GetInstance();
	rowRepository = RowRepository::GetInstance();

	id = _id;
	subscriber = subscriberRepository->Object(_subscriberId);
	isBlocked = _isBlocked;
}

void ReaderCard::AddRow(Book book)
{
	DateTime now = DateTimeHelper::Now();
	Row* r = new Row(book, now, DateTimeHelper::AddMonth(now));
	rowRepository->Create(r);
}

std::vector<Row*> ReaderCard::Rows()
{
	return rowRepository->Collection();
}

void ReaderCard::SwitchIsBlocked()
{
	isBlocked = !isBlocked;
}


//ReaderCardRepository
ReaderCardRepository* ReaderCardRepository::obj = nullptr;

void ReaderCardRepository::Build()
{
	std::ifstream f("reader_cards.json");

	std::string json = FileHelper::Read(&f);

	auto collection = JsonHelper::Parse(json);

	for (auto row : collection)
	{
		ReaderCard *tmp = new ReaderCard(StringHelper::ToInt(row["id"]), StringHelper::ToInt(row["subscriberId"]), StringHelper::ToBool(row["isBlocked"]));
		readerCards.push_back(tmp);
	}
	f.close();
}

ReaderCardRepository::ReaderCardRepository()
{
	subscriberRepository = SubscriberRepository::GetInstance();
	obj = this;
	Build();
}

ReaderCardRepository* ReaderCardRepository::GetInstance()
{
	if (obj == nullptr)
		return new ReaderCardRepository();
	else
		return obj;
}

ReaderCard ReaderCardRepository::Object(Subscriber subscriber)
{
	for (auto card : readerCards)
		if (card->subscriber.id == subscriber.id)
			return *card;
}

ReaderCard ReaderCardRepository::Object(int subscriberId)
{
	for (auto card : readerCards)
		if (card->subscriber.id == subscriberId)
			return *card;
}

std::vector<ReaderCard*> ReaderCardRepository::Collection()
{
	return readerCards;
}

ReaderCard ReaderCardRepository::Create(std::string username)
{
	Subscriber s = subscriberRepository->Create(username);
	ReaderCard *c = new ReaderCard(s);
	readerCards.push_back(c);

	return *c;
}

void ReaderCardRepository::Delete(Subscriber subscriber)
{
	for (int i = 0; i < readerCards.size(); i++)
		if (readerCards[i]->subscriber.id == subscriber.id)
		{
			readerCards.erase(readerCards.begin() + i);
			subscriberRepository->Delete(subscriber);
			break;
		}
}

void ReaderCardRepository::AddRow(Subscriber subscriber, Book book)
{
	for (auto card : readerCards)
		if (card->subscriber.id == subscriber.id)
		{
			card->AddRow(book);
			break;
		}
}


// Row
Row::Row() {
	
}

Row::Row(Book _book, DateTime from, DateTime to)
{
	book = _book;
	dateFrom = from;
	dateTo = to;
	isReturned = false;
}

Row::Row(int _id, int _bookId, int _readerCardId, DateTime from, DateTime to, bool _isRet)
{
	bookRepository = BookRepository::GetInstance();
	readerCardRepository = ReaderCardRepository::GetInstance();

	id = _id;
	book = bookRepository->Object(_bookId);
	readerCard = readerCardRepository->Object(_readerCardId);
	dateFrom = from;
	dateTo = to;
	isReturned = _isRet;
}

bool Row::IsExpired()
{
	return DateTimeHelper::Compare(DateTimeHelper::Now(), dateTo) == 1;
}


// RowRepository
RowRepository* RowRepository::obj = nullptr;

void RowRepository::Build()
{
	std::ifstream f("rows.json");

	std::string json = FileHelper::Read(&f);

	auto collection = JsonHelper::Parse(json);

	for (auto row : collection)
	{
		Row* tmp = new Row(StringHelper::ToInt(row["id"]), StringHelper::ToInt(row["bookId"]), StringHelper::ToInt(row["readerCardId"]), DateTimeHelper::Parse(row["dateFrom"]), DateTimeHelper::Parse(row["dateTo"]), StringHelper::ToBool(row["isReturned"]));
		rows.push_back(tmp);
	}
	f.close();

}

RowRepository::RowRepository()
{
	obj = this;
	Build();
}

RowRepository* RowRepository::GetInstance()
{
	if (obj == nullptr)
		return new RowRepository();
	else
		return obj;
}

Row RowRepository::Object(int rowId)
{
	for (auto row : rows)
		if (row->id == rowId)
			return *row;
}

void RowRepository::Create(Row* r)
{
	rows.push_back(r);
}

std::vector<Row*> RowRepository::Collection()
{
	return rows;
}


// Subcsriber
Subscriber::Subscriber()
{

}

Subscriber::Subscriber(std::string _name) { name = _name; }

Subscriber::Subscriber(int _id, std::string _name) { name = _name; id = _id; }


// SubscriberRepository
SubscriberRepository* SubscriberRepository::obj = nullptr;

void SubscriberRepository::Build()
{
	std::ifstream f("subscribers.json");

	std::string json = FileHelper::Read(&f);

	auto collection = JsonHelper::Parse(json);

	for (auto row : collection)
	{
		Subscriber *tmp = new Subscriber(StringHelper::ToInt(row["id"]), row["name"]);
		subscribers.push_back(tmp);
	}
	f.close();

}

SubscriberRepository::SubscriberRepository()
{
	obj = this;
	Build();
}

SubscriberRepository* SubscriberRepository::GetInstance()
{
	if (obj == NULL)
		return new SubscriberRepository();
	else
		return obj;
}

Subscriber SubscriberRepository::Object(int id)
{
	for (auto sub : subscribers)
		if (sub->id == id)
			return *sub;
}

Subscriber SubscriberRepository::Create(std::string name)
{
	Subscriber *s = new Subscriber(name);
	subscribers.push_back(s);
	return *s;
}

void SubscriberRepository::Delete(Subscriber subscriber)
{
	for (int i = 0; i < subscribers.size(); i++)
		if (subscribers[i]->id == subscriber.id)
		{
			subscribers.erase(subscribers.begin() + i);
			return;
		}
}


// StringHelper
std::string StringHelper::ToString(int num)
{
	char* n = new char[9];
	n += 8;
	*n = '\0';
	n--;

	while (num != 0)
	{
		*n-- = num % 10;
		num /= 10;
	}

	return std::string(n);
}

int StringHelper::ToInt(std::string s)
{
	int a = 0;
	auto current = --s.end();
	while (true)
	{
		a = a * 10 + *current - '0';
		if (current == s.begin())
			break;
		current--;
	}

	return a;
}

bool StringHelper::ToBool(std::string s)
{
	return s.compare("false");
}

// JsonHelper
std::vector<std::map<std::string, std::string>> JsonHelper::Parse(std::string json)
{
	std::vector<std::map<std::string, std::string>> objs;
	std::map<std::string, std::string> obj;

	bool isString = false;
	bool isKey = false;
	bool isValueWaited = false;
	bool isValue = false;

	std::string key = "";
	std::string value = "";

	for (char c : json)
	{
		if (c == '}')
		{
			objs.push_back(obj);
			obj.clear();
		}

		if (c == '\"')
		{
			if (isString && isValue)
			{
				obj.insert(std::pair<std::string, std::string>(key, value));
				isKey = isValue = isValueWaited = false;
				key = value = "";
			}
			else if (isKey)
			{
				isKey = false;
			}
			else if (!isKey && !isValueWaited)
			{
				isKey = true;
			}
			else if (isValueWaited)
			{
				isValue = true;
			}

			isString = isString ^ true;

			continue;
		}

		if (isString)
		{
			if (isKey)
				key = key + c;
			else
				value = value + c;
		}

		if (c == ':')
			isValueWaited = true;
	}

	return objs;
}


// FileHelper
std::string FileHelper::Read(std::ifstream* f)
{
	f->seekg(0, std::ios::end);
	size_t size = f->tellg();
	std::string buffer(size, ' ');
	f->seekg(0);
	f->read(&buffer[0], size);

	return buffer;
}


// DateTimeHelper
int DateTimeHelper::Compare(DateTime a, DateTime b)
{
	if (a.year > b.year)
		return 1;
	else if (a.year < b.year)
		return 2;

	if (a.month > b.month)
		return 1;
	else if (a.month < b.month)
		return 2;

	if (a.day > b.day)
		return 1;
	else if (a.day < b.day)
		return 2;

	return 0;
}

DateTime DateTimeHelper::AddMonth(DateTime d)
{
	int year = d.month == 12 ? d.year + 1 : d.year;
	int month = d.month == 12 ? 1 : d.month + 1;

	return DateTime(d.day, month, year);
}

DateTime DateTimeHelper::Now()
{
	std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);
	return DateTime(now->tm_mday, now->tm_mon, now->tm_year);
}

DateTime DateTimeHelper::Parse(std::string s)
{
	int datetime[3] = { 0,0,0 };
	int i = 0;

	int a = 0;
	auto current = s.begin();
	while (true)
	{
		if (current == s.end() || *current == '.')
		{
			datetime[i] = a;
			if (current == s.end())
				break;

			a = 0;
			i++;
			current++;
			continue;
		}

		a = a * 10 + *current - '0';

		current++;
	}

	return DateTime(datetime[0], datetime[1], datetime[2]);

}


//Exception
PermissionDeniedException::PermissionDeniedException() : Exception(PERMISSION_DENIED_FOR_THIS_ACTION)
{

}

Exception::Exception(std::string _msg) { msg = _msg; }


// DateTime
DateTime::DateTime()
{

}

DateTime::DateTime(int _d, int _m, int _y)
{
	day = _d;
	month = _m;
	year = _y;
}


// Book
Book::Book()
{

}

Book::Book(int _id, std::string _title, std::string _author)
{
	id = _id;
	title = _title;
	author = _author;
}


// BookRepository
BookRepository* BookRepository::obj = nullptr;

void BookRepository::Build()
{
	std::ifstream f("books.json");

	std::string json = FileHelper::Read(&f);

	auto collection = JsonHelper::Parse(json);

	for (auto row : collection)
	{
		Book tmp = Book(StringHelper::ToInt(row["id"]), row["title"], row["author"]);
		books.push_back(tmp);
	}

	f.close();
}

BookRepository::BookRepository()
{
	obj = this;
	Build();
}

BookRepository* BookRepository::GetInstance()
{
	if (obj == nullptr)
		return new BookRepository();
	else
		return obj;
}

Book BookRepository::Object(int id)
{
	for (auto book : books)
		if (book.id == id)
			return book;
}

std::vector<Book> BookRepository::Collection()
{
	return books;
}


int main()
{
	RowRepository* r = RowRepository::GetInstance();
	ReaderCardRepository* r2 = ReaderCardRepository::GetInstance();
	SubscriberRepository* sr = SubscriberRepository::GetInstance();
	BookRepository* br = BookRepository::GetInstance();
    return 0;
}
