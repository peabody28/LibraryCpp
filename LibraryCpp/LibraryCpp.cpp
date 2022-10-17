#include "DateTime.h"
#include "Book.h"
#include "Row.h"
#include "ReaderCard.h"
#include "Subscriber.h"

#include "WorkmanResolveOperation.h"

#include "BookRepository.h"
#include "RowRepository.h"
#include "SubscriberRepository.h"
#include "ReaderCardRepository.h"
#include "WorkmanRepository.h"

#include "WorkmanTypeHelper.h"
#include "DateTimeHelper.h"
#include "StringHelper.h"
#include "FileHelper.h"
#include "JsonHelper.h"

#include "PermissionDeniedException.h"
#include "Exception.h"

#include "Assistant.h"
#include "Manager.h"
#include "Director.h"

#include "Library.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <map>

#define UPDATE_REPOSITORIES_DATA_DELAY 10000 // 10 seconds

#define PERMISSION_DENIED_FOR_THIS_ACTION "PERMISSION_DENIED_FOR_THIS_ACTION"

std::ostream& operator << (std::ostream& os, const DateTime& d)
{
	return os << d.day << " " << d.month << " " << d.year;
}

std::ostream& operator << (std::ostream& os, const Subscriber& s)
{
	return os << s.id << " " << s.name;
}

std::ostream& operator << (std::ostream& os, const Book& s)
{
	return os << s.id << " " << s.author << " " << s.title;
}

#pragma region Book

Book::Book()
{

}

Book::Book(int _id, std::string _title, std::string _author)
{
	id = _id;
	title = _title;
	author = _author;
}

#pragma endregion Book

#pragma region ReaderCard

ReaderCard::ReaderCard() {
	subscriberRepository = SubscriberRepository::GetInstance();
	rowRepository = RowRepository::GetInstance();
}

ReaderCard::ReaderCard(Subscriber* sub)
{
	subscriberRepository = SubscriberRepository::GetInstance();
	rowRepository = RowRepository::GetInstance();

	subscriberId = sub->id;
	isBlocked = false;
}

ReaderCard::ReaderCard(int _id, int _subscriberId, bool _isBlocked)
{
	subscriberRepository = SubscriberRepository::GetInstance();
	rowRepository = RowRepository::GetInstance();

	id = _id;
	subscriberId =_subscriberId;
	isBlocked = _isBlocked;
}

Subscriber* ReaderCard::_Subscriber()
{
	return subscriberRepository->Object(subscriberId);
}

void ReaderCard::AddRow(Book* book)
{
	DateTime now = DateTimeHelper::Now();
	Row* r = new Row(book, this, now, DateTimeHelper::AddMonth(now));
	rowRepository->Create(r);
}

std::vector<Row*> ReaderCard::Rows(ReaderCard* readerCard)
{
	return rowRepository->Collection(readerCard->id);
}

void ReaderCard::SwitchIsBlocked()
{
	isBlocked = !isBlocked;
}

#pragma endregion ReaderCard

#pragma region ReaderCardRepository

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

void ReaderCardRepository::UpdateData()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_REPOSITORIES_DATA_DELAY));
		std::vector<ReaderCard*> tmp = ReaderCardRepository::obj->readerCards;
		ReaderCardRepository::obj->Build();
	}
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
	return obj;
}

ReaderCard* ReaderCardRepository::Object(Subscriber* subscriber)
{
	for (auto card : readerCards)
		if (card->_Subscriber()->id == subscriber->id)
			return card;
	return NULL;
}

ReaderCard* ReaderCardRepository::Object(int subscriberId)
{
	for (auto card : readerCards)
		if (card->_Subscriber()->id == subscriberId)
			return card;
	return NULL;
}

std::vector<ReaderCard*> ReaderCardRepository::Collection()
{
	return readerCards;
}

ReaderCard* ReaderCardRepository::Create(std::string username)
{
	Subscriber* s = subscriberRepository->Create(username);
	ReaderCard *c = new ReaderCard(s);
	readerCards.push_back(c);

	return c;
}

void ReaderCardRepository::Delete(Subscriber* subscriber)
{
	for (int i = 0; i < readerCards.size(); i++)
		if (readerCards[i]->_Subscriber()->id == subscriber->id)
		{
			readerCards.erase(readerCards.begin() + i);
			subscriberRepository->Delete(subscriber);
			break;
		}
}

void ReaderCardRepository::AddRow(Subscriber* subscriber, Book* book)
{
	for (auto card : readerCards)
		if (card->_Subscriber()->id == subscriber->id)
		{
			card->AddRow(book);
			break;
		}
}

#pragma endregion ReaderCardRepository

#pragma region Row

Row::Row() {
	bookRepository = BookRepository::GetInstance();
	readerCardRepository = ReaderCardRepository::GetInstance();
}

Row::Row(Book* book, ReaderCard* readerCard, DateTime f, DateTime t)
{
	bookRepository = BookRepository::GetInstance();
	readerCardRepository = ReaderCardRepository::GetInstance();

	bookId = book->id;
	readerCardId = readerCard->id;
	dateFrom = f;
	dateTo = t;
	isReturned = false;
}

Row::Row(int _id, int _bookId, int _readerCardId, DateTime from, DateTime to, bool _isRet)
{
	bookRepository = BookRepository::GetInstance();
	readerCardRepository = ReaderCardRepository::GetInstance();

	id = _id;
	bookId = _bookId;
	readerCardId = _readerCardId;
	dateFrom = from;
	dateTo = to;
	isReturned = _isRet;
}

Book* Row::_Book()
{
	return bookRepository->Object(bookId);
}

ReaderCard* Row::_ReaderCard()
{
	return readerCardRepository->Object(readerCardId);
}

bool Row::IsExpired()
{
	return DateTimeHelper::Compare(DateTimeHelper::Now(), dateTo) == 1;
}

#pragma endregion Row

#pragma region RowRepository

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

void RowRepository::UpdateData()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_REPOSITORIES_DATA_DELAY));
		std::vector<Row*> tmp = RowRepository::obj->rows;
		RowRepository::obj->Build();
	}
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
	return obj;
}

Row* RowRepository::Object(int rowId)
{
	for (auto row : rows)
		if (row->id == rowId)
			return row;
	return NULL;
}

void RowRepository::Create(Row* r)
{
	rows.push_back(r);
}

std::vector<Row*> RowRepository::Collection(int readerCardId)
{
	std::vector<Row*> collection;
	for (auto item : rows)
		if (item->readerCardId == readerCardId)
			collection.push_back(item);
	return collection;
}

#pragma endregion RowRepository

#pragma region Subscriber

Subscriber::Subscriber()
{

}

Subscriber::Subscriber(std::string _name) { name = _name; }

Subscriber::Subscriber(int _id, std::string _name) { name = _name; id = _id; }

#pragma endregion Subscriber

#pragma region SubscriberRepository

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

void SubscriberRepository::UpdateData()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_REPOSITORIES_DATA_DELAY));
		std::vector<Subscriber*> tmp = SubscriberRepository::obj->subscribers;
		SubscriberRepository::obj->Build();
	}
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
	return obj;
}

Subscriber* SubscriberRepository::Object(int id)
{
	for (auto sub : subscribers)
		if (sub->id == id)
			return sub;
	return NULL;
}

Subscriber* SubscriberRepository::Create(std::string name)
{
	Subscriber *s = new Subscriber(name);
	subscribers.push_back(s);
	return s;
}

void SubscriberRepository::Delete(Subscriber* subscriber)
{
	for (int i = 0; i < subscribers.size(); i++)
		if (subscribers[i]->id == subscriber->id)
		{
			subscribers.erase(subscribers.begin() + i);
			return;
		}
}

#pragma endregion SubscriberRepository

#pragma region StringHelper

std::string StringHelper::ToString(int num)
{
	char* n = new char[9];
	n += 8;
	*n = '\0';
	n--;

	while (num != 0)
	{
		*n-- = (num % 10) + '0';
		num /= 10;
	}
	n++;
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

#pragma endregion StringHelper

#pragma region JsonHelper

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

std::string JsonHelper::Serialize(std::map<std::string, std::string> obj)
{
	std::string json = "{";

	for (auto row : obj)
		json = json + "\"" + row.first + "\"" + ":" + "\"" + row.second + "\"" + ",";

	json += "}";

	return json;
}

std::string JsonHelper::SerializeArray(std::vector< std::map<std::string, std::string>> arr)
{
	std::string s = "[";
	for (auto obj : arr)
		s += Serialize(obj) + ",";
	s += "]";

	return s;
}

#pragma endregion JsonHelper

#pragma region FileHelper

std::string FileHelper::Read(std::ifstream* f)
{
	f->seekg(0, std::ios::end);
	size_t size = f->tellg();
	std::string buffer(size, ' ');
	f->seekg(0);
	f->read(&buffer[0], size);

	return buffer;
}

#pragma endregion FileHelper

#pragma region DateTimeHelper

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
	return DateTime(now->tm_mday, now->tm_mon, now->tm_year+1900);
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

#pragma endregion DateTimeHelper

#pragma region Exception

PermissionDeniedException::PermissionDeniedException() : Exception(PERMISSION_DENIED_FOR_THIS_ACTION)
{

}

Exception::Exception(std::string _msg) { msg = _msg; }

#pragma endregion Exception

#pragma region DateTime

DateTime::DateTime()
{

}

DateTime::DateTime(int _d, int _m, int _y)
{
	day = _d;
	month = _m;
	year = _y;
}

#pragma endregion DateTime

#pragma region BookRepository

BookRepository* BookRepository::obj = nullptr;

void BookRepository::Build()
{
	std::ifstream f("books.json");

	std::string json = FileHelper::Read(&f);

	auto collection = JsonHelper::Parse(json);

	for (auto row : collection)
	{
		Book* tmp = new Book(StringHelper::ToInt(row["id"]), row["title"], row["author"]);
		books.push_back(tmp);
	}

	f.close();
}

void BookRepository::UpdateData()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_REPOSITORIES_DATA_DELAY));
		std::vector<Book*> tmp = BookRepository::obj->books;
		BookRepository::obj->Build();
	}
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
	return obj;
}

Book* BookRepository::Object(int id)
{
	for (auto book : books)
		if (book->id == id)
			return book;
	return NULL;
}

std::vector<Book*> BookRepository::Collection()
{
	return books;
}

#pragma endregion BookRepository

#pragma region WormanTypeHelper

WorkmanType WorkmanTypeHelper::Parse(std::string type)
{
	if (!type.compare("Assistant"))
		return WorkmanType::Assistant;
	else if (!type.compare("Director"))
		return WorkmanType::Director;
	else if (!type.compare("Manager"))
		return WorkmanType::Manager;

	return WorkmanType::Assistant;
}

std::string WorkmanTypeHelper::ToString(WorkmanType w)
{
	switch (w)
	{
	case Assistant:
		return "Assistant";
		break;
	case Manager:
		return "Manager";
		break;
	case Director:
		return "Director";
		break;
	}
}

#pragma endregion WorkmanTypeHelper

#pragma region WorkmanRepository

WorkmanRepository* WorkmanRepository::obj = nullptr;

void WorkmanRepository::Build()
{
	std::ifstream f("workmans.json");

	std::string json = FileHelper::Read(&f);

	auto collection = JsonHelper::Parse(json);

	staff.clear();
	for (auto row : collection)
	{
		Workman* tmp = WorkmanResolveOperation::Resolve(WorkmanTypeHelper::Parse(row["type"]));
		tmp->id = StringHelper::ToInt(row["id"]);
		staff.push_back(tmp);
	}
	f.close();
}

void WorkmanRepository::UpdateData()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_REPOSITORIES_DATA_DELAY));

		std::vector<Workman*> tmp = WorkmanRepository::obj->staff; // save staff from memory
		WorkmanRepository::obj->Build(); // load staff from file

		// update staff (memory + file)
		for (auto item : tmp)
			if (std::find_if(WorkmanRepository::obj->staff.begin(), WorkmanRepository::obj->staff.end(),
				[&](Workman* obj) { return obj->id == item->id; }) == WorkmanRepository::obj->staff.end())
			{
				WorkmanRepository::obj->staff.push_back(item);
			}

		// serialize staff array
		std::vector< std::map<std::string, std::string>> arr;

		for (auto obj : WorkmanRepository::obj->staff)
		{
			std::map<std::string, std::string> o;
			o.insert(std::pair<std::string, std::string>("id", StringHelper::ToString(obj->id)));
			o.insert(std::pair<std::string, std::string>("type", WorkmanTypeHelper::ToString(obj->type)));
			arr.push_back(o);
		}

		std::string json = JsonHelper::SerializeArray(arr);

		std::ofstream f("workmans.json");

		f << json;

		f.close();
	}
}

WorkmanRepository::WorkmanRepository() 
{
	obj = this;
	Build();
}

WorkmanRepository* WorkmanRepository::GetInstance()
{
	if (obj == nullptr)
		return new WorkmanRepository();
	return obj;
}

Workman* WorkmanRepository::Object(int _id)
{
	for (auto man : staff)
		if (man->id == _id)
			return man;
	return NULL;
}

Workman* WorkmanRepository::Add(int _id, WorkmanType w)
{
	Workman* workman = WorkmanResolveOperation::Resolve(w);
	workman->id = _id;

	staff.push_back(workman);
	return workman;
}

#pragma endregion WorkmanRepository

#pragma region Library

Library::Library()
{
	bookRepository = BookRepository::GetInstance();
	subscriberRepository = SubscriberRepository::GetInstance();
	readerCardRepository = ReaderCardRepository::GetInstance();
	workmanRepository = WorkmanRepository::GetInstance();
}

void Library::PrintBooks()
{
	for (auto book : bookRepository->Collection())
		std::cout << *book << std::endl;
}

void Library::PrintSubscribers()
{
	for (auto card : readerCardRepository->Collection())
		std::cout << *(card->_Subscriber()) << std::endl;
}

void Library::PrintBooks(int _subscriberId)
{
	for (auto card : readerCardRepository->Collection())
		if (card->_Subscriber()->id == _subscriberId)
		{
			for (auto row : card->Rows(card))
				std::cout << *(row->_Book()) << " " << "dateFrom: " << row->dateFrom << " DateTo: " << row->dateTo << std::endl;
			break;
		}
}

void Library::Debts()
{
	for (auto card : readerCardRepository->Collection())
		for (auto row : card->Rows(card))
		{
			if (row->IsExpired())
				std::cout << *(card->_Subscriber()) << " " << *(row->_Book()) << " " << "dateFrom: " << row->dateFrom << " DateTo: " << row->dateTo << std::endl;
		}
}

ReaderCard* Library::AddSubcriber(std::string username)
{
	return readerCardRepository->Create(username);
}

void Library::SwitchSubscriberState(std::string name, Workman workman)
{
	if (workman.type != Manager && workman.type != Director)
		throw PermissionDeniedException();

	for (auto card : readerCardRepository->Collection())
		if (!card->_Subscriber()->name.compare(name))
		{
			card->SwitchIsBlocked();
			break;
		}
}

void Library::DeleteSubscriber(std::string name, Workman workman)
{
	if (workman.type != Director)
		throw PermissionDeniedException();

	for (auto card : readerCardRepository->Collection())
		if (!card->_Subscriber()->name.compare(name))
		{
			readerCardRepository->Delete(card->_Subscriber());
			break;
		}
}

void Library::GiveBook(Subscriber* subscriber, Book* book, Workman workman)
{
	if (workman.type != Assistant)
		throw PermissionDeniedException();

	readerCardRepository->AddRow(subscriber, book);
}

#pragma endregion Library

#pragma region Workman

Workman::Workman()
{

}

Workman::Workman(WorkmanType w)
{
	type = w;
}

staff::Assistant::Assistant() : Workman(WorkmanType::Assistant) {}

staff::Manager::Manager() : Workman(WorkmanType::Manager)
{

}

staff::Manager::Manager(WorkmanType w) : Workman(w) { }

staff::Director::Director() : Manager(WorkmanType::Director) {}

#pragma endregion Workman

#pragma region WormanResolveOperation

Workman* WorkmanResolveOperation::Resolve(WorkmanType w)
{
	switch (w)
	{
	case Assistant:
		return new staff::Assistant();
		break;
	case Manager:
		return new staff::Manager();
		break;
	case Director:
		return new staff::Director();
		break;
	default:
		return NULL;
		break;
	}
}

#pragma endregion WorkmanResolveOperation

int main()
{
	WorkmanRepository* rr = WorkmanRepository::GetInstance();
	Library l = Library();

	ReaderCard* rc = l.AddSubcriber("Max");
	Book* book = l.bookRepository->Object(1);
	Workman* w = WorkmanResolveOperation::Resolve(WorkmanType::Assistant);

	l.GiveBook(rc->_Subscriber(), book, *w);

	l.PrintBooks(rc->_Subscriber()->id);

	std::thread t1(WorkmanRepository::UpdateData);
	std::thread t2(BookRepository::UpdateData);
	std::thread t3(SubscriberRepository::UpdateData);
	std::thread t4(RowRepository::UpdateData);
	std::thread t5(ReaderCardRepository::UpdateData);
	
	while (true)
	{
		int id;
		std::cin >> id;

		rr->Add(id, WorkmanType::Assistant);
	}
    return 0;
}
