#include "DateTime.h"
#include "Book.h"
#include "Row.h"
#include "ReaderCard.h"
#include "Subscriber.h"

#include "WorkmanResolveOperation.h"

#include "IRepository.h"
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

#include "ActionType.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <map>
#include <mutex>
#include <functional>

#define UPDATE_REPOSITORIES_DATA_DELAY 2000

#define PERMISSION_DENIED_FOR_THIS_ACTION "PERMISSION_DENIED_FOR_THIS_ACTION"

std::mutex g_lock;

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

	id = ReaderCardRepository::GetInstance()->GenerateId();
	subscriberId = sub->id;
	isBlocked = false;
}

ReaderCard::ReaderCard(int _id, int _subscriberId, bool _isBlocked, bool _isDeleted)
{
	subscriberRepository = SubscriberRepository::GetInstance();
	rowRepository = RowRepository::GetInstance();

	id = _id;
	subscriberId =_subscriberId;
	isBlocked = _isBlocked;
	isDeleted = _isDeleted;
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

std::vector<Row*> ReaderCard::Rows()
{
	return rowRepository->Collection(this->id);
}

void ReaderCard::SwitchIsBlocked()
{
	isBlocked = !isBlocked;
}

#pragma endregion ReaderCard

#pragma region ReaderCardRepository

ReaderCardRepository* ReaderCardRepository::obj = nullptr;

std::vector<ReaderCard*> ReaderCardRepository::Build()
{
	try
	{
		std::ifstream f(filename);

		std::string json = FileHelper::Read(&f);

		auto collection = JsonHelper::Parse(json);

		std::vector<ReaderCard*> arr;

		for (auto row : collection)
		{
			ReaderCard* tmp = new ReaderCard(StringHelper::ToInt(row["id"]), StringHelper::ToInt(row["subscriberId"]), StringHelper::ToBool(row["isBlocked"]), StringHelper::ToBool(row["isDeleted"]));
			arr.push_back(tmp);
		}
		f.close();

		return arr;
	}
	catch (const char* msg)
	{
		std::cout << std::string(msg);
	}
}

std::map<std::string, std::string>  ReaderCardRepository::Serialize(ReaderCard* obj)
{
	std::map<std::string, std::string> o;
	o.insert(std::pair<std::string, std::string>("id", StringHelper::ToString(obj->id)));
	o.insert(std::pair<std::string, std::string>("subscriberId", StringHelper::ToString(obj->subscriberId)));
	o.insert(std::pair<std::string, std::string>("isBlocked", StringHelper::ToString((int)obj->isBlocked)));
	o.insert(std::pair<std::string, std::string>("isDeleted", StringHelper::ToString((int)obj->isDeleted)));

	return o;
}

ReaderCardRepository::ReaderCardRepository()
{
	filename = "reader_cards.json";
	subscriberRepository = SubscriberRepository::GetInstance();
	obj = this;
	repository = Build();
}

ReaderCardRepository* ReaderCardRepository::GetInstance()
{
	if (obj == nullptr)
		return new ReaderCardRepository();
	return obj;
}

ReaderCard* ReaderCardRepository::Object(std::string username)
{
	for (auto card : repository)
		if (!card->_Subscriber()->name.compare(username) && !card->isBlocked && !card->isDeleted)
			return card;
	return NULL;
}

ReaderCard* ReaderCardRepository::Object(int subscriberId)
{
	for (auto card : repository)
		if (card->_Subscriber()->id == subscriberId && !card->isDeleted)
			return card;
	return NULL;
}

std::vector<ReaderCard*> ReaderCardRepository::Collection()
{
	std::vector<ReaderCard*> collection;
	for (auto item : repository)
		if (!item->isDeleted)
			collection.push_back(item);
	return collection;
}

bool ReaderCardRepository::AlreadyTaken(Book* book)
{
	for (auto rc : repository)
	{
		for (auto r : rc->Rows())
		{
			if (r->bookId == book->id && !r->isReturned)
				return true;
		}
	}

	return false;
}

ReaderCard* ReaderCardRepository::Create(std::string username)
{
	Subscriber* s = subscriberRepository->Create(username);
	ReaderCard *c = new ReaderCard(s);
	repository.push_back(c);

	return c;
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

	id = RowRepository::GetInstance()->GenerateId();
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

std::vector<Row*> RowRepository::Build()
{
	try
	{
		std::ifstream f(filename);

		std::string json = FileHelper::Read(&f);

		auto collection = JsonHelper::Parse(json);

		std::vector<Row*> arr;
		for (auto row : collection)
		{
			Row* tmp = new Row(StringHelper::ToInt(row["id"]), StringHelper::ToInt(row["bookId"]), StringHelper::ToInt(row["readerCardId"]), DateTimeHelper::Parse(row["dateFrom"]), DateTimeHelper::Parse(row["dateTo"]), StringHelper::ToBool(row["isReturned"]));
			arr.push_back(tmp);
		}
		f.close();

		return arr;
	}
	catch (const char* msg)
	{
		std::cout << std::string(msg);
	}
}

std::map<std::string, std::string>  RowRepository::Serialize(Row* obj)
{
	std::map<std::string, std::string> o;
	o.insert(std::pair<std::string, std::string>("id", StringHelper::ToString(obj->id)));
	o.insert(std::pair<std::string, std::string>("bookId", StringHelper::ToString(obj->bookId)));
	o.insert(std::pair<std::string, std::string>("readerCardId", StringHelper::ToString(obj->readerCardId)));
	o.insert(std::pair<std::string, std::string>("dateFrom", DateTimeHelper::ToString(obj->dateFrom)));
	o.insert(std::pair<std::string, std::string>("dateTo", DateTimeHelper::ToString(obj->dateTo)));
	o.insert(std::pair<std::string, std::string>("isReturned", StringHelper::ToString((int)obj->isReturned)));

	return o;
}

RowRepository::RowRepository()
{
	filename = "rows.json";
	obj = this;
	repository = Build();
}

RowRepository* RowRepository::GetInstance()
{
	if (obj == nullptr)
		return new RowRepository();
	return obj;
}

Row* RowRepository::Object(int rowId)
{
	for (auto row : repository)
		if (row->id == rowId)
			return row;
	return NULL;
}

void RowRepository::Create(Row* r)
{
	repository.push_back(r);
}

std::vector<Row*> RowRepository::Collection(int readerCardId)
{
	std::vector<Row*> collection;
	for (auto item : repository)
		if (item->readerCardId == readerCardId)
			collection.push_back(item);
	return collection;
}

#pragma endregion RowRepository

#pragma region Subscriber

Subscriber::Subscriber()
{

}

Subscriber::Subscriber(std::string _name) { id = SubscriberRepository::GetInstance()->GenerateId(), name = _name; }

Subscriber::Subscriber(int _id, std::string _name) { name = _name; id = _id; }

#pragma endregion Subscriber

#pragma region SubscriberRepository

SubscriberRepository* SubscriberRepository::obj = nullptr;

std::vector<Subscriber*> SubscriberRepository::Build()
{
	try
	{
		std::ifstream f(filename);

		std::string json = FileHelper::Read(&f);

		auto collection = JsonHelper::Parse(json);

		std::vector<Subscriber*> arr;

		for (auto row : collection)
		{
			Subscriber* tmp = new Subscriber(StringHelper::ToInt(row["id"]), row["name"]);
			arr.push_back(tmp);
		}
		f.close();

		return arr;
	}
	catch(const char* msg)
	{
		std::cout <<std::string(msg);
	}
	
}

std::map<std::string, std::string> SubscriberRepository::Serialize(Subscriber* obj)
{
	std::map<std::string, std::string> o;
	o.insert(std::pair<std::string, std::string>("id", StringHelper::ToString(obj->id)));
	o.insert(std::pair<std::string, std::string>("name", obj->name));
	return o;
}

SubscriberRepository::SubscriberRepository()
{
	filename = "subscribers.json";
	obj = this;
	repository = Build();
}

SubscriberRepository* SubscriberRepository::GetInstance()
{
	if (obj == NULL)
		return new SubscriberRepository();
	return obj;
}

Subscriber* SubscriberRepository::Object(int id)
{
	for (auto sub : repository)
		if (sub->id == id)
			return sub;
	return NULL;
}

Subscriber* SubscriberRepository::Object(std::string name)
{
	for (auto sub : repository)
		if (!sub->name.compare(name))
			return sub;
	return NULL;
}

Subscriber* SubscriberRepository::Create(std::string name)
{
	Subscriber *s = new Subscriber(name);
	repository.push_back(s);
	return s;
}

#pragma endregion SubscriberRepository

#pragma region StringHelper

std::string StringHelper::ToString(int num)
{
	char* n = new char[9];
	n += 8;
	*n = '\0';
	n--;

	do
	{
		*n-- = (num % 10) + '0';
		num /= 10;
	} while (num != 0);
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
	return s.compare("1") == 0;
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
	return DateTime(now->tm_mday, now->tm_mon+1, now->tm_year+1900);
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

std::string DateTimeHelper::ToString(DateTime& date)
{
	return StringHelper::ToString(date.day) + "." + StringHelper::ToString(date.month) + "." + StringHelper::ToString(date.year);
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

std::vector<Book*> BookRepository::Build()
{
	try
	{
		std::ifstream f(filename);

		std::string json = FileHelper::Read(&f);

		auto collection = JsonHelper::Parse(json);

		std::vector<Book*> arr;

		for (auto row : collection)
		{
			Book* tmp = new Book(StringHelper::ToInt(row["id"]), row["title"], row["author"]);
			arr.push_back(tmp);
		}

		f.close();
		return arr;
	}
	catch (const char* msg)
	{
		std::cout << std::string(msg);
	}
	
}

std::map<std::string, std::string> BookRepository::Serialize(Book* obj)
{
	std::map<std::string, std::string> o;
	o.insert(std::pair<std::string, std::string>("id", StringHelper::ToString(obj->id)));
	o.insert(std::pair<std::string, std::string>("title", obj->title));
	o.insert(std::pair<std::string, std::string>("author", obj->author));

	return o;
}

BookRepository::BookRepository()
{
	filename = "books.json";
	obj = this;
	repository = Build();
}

BookRepository* BookRepository::GetInstance()
{
	if (obj == nullptr)
		return new BookRepository();
	return obj;
}

Book* BookRepository::Object(int _bookId)
{
	for (auto book : repository)
		if (book->id == _bookId)
			return book;
	return NULL;
}

Book* BookRepository::Object(std::string title)
{
	for (auto book : repository)
		if (!book->title.compare(title))
		{
			ReaderCardRepository* rcr = ReaderCardRepository::GetInstance();
			
			return rcr->AlreadyTaken(book) ? NULL : book;
		}
			
	return NULL;
}

std::vector<Book*> BookRepository::Collection()
{
	return repository;
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

std::vector<Workman*> WorkmanRepository::Build()
{
	try
	{
		std::ifstream f(filename);

		std::string json = FileHelper::Read(&f);

		auto collection = JsonHelper::Parse(json);

		std::vector<Workman*> arr;
		for (auto row : collection)
		{
			Workman* tmp = WorkmanResolveOperation::Resolve(WorkmanTypeHelper::Parse(row["type"]));
			tmp->id = StringHelper::ToInt(row["id"]);
			arr.push_back(tmp);
		}
		f.close();
		return arr;
	}
	catch (const char* msg)
	{
		std::cout << std::string(msg);
	}
	
}

std::map<std::string, std::string> WorkmanRepository::Serialize(Workman* obj)
{
	std::map<std::string, std::string> o;
	o.insert(std::pair<std::string, std::string>("id", StringHelper::ToString(obj->id)));
	o.insert(std::pair<std::string, std::string>("type", WorkmanTypeHelper::ToString(obj->type)));
	return o;
}

WorkmanRepository::WorkmanRepository() 
{
	filename = "workmans.json";
	obj = this;
	repository = Build();
}

WorkmanRepository* WorkmanRepository::GetInstance()
{
	if (obj == nullptr)
		return new WorkmanRepository();
	return obj;
}

Workman* WorkmanRepository::Object(int id)
{
	for (auto w : repository)
		if (w->id == id)
			return w;
	return NULL;
}

Workman* WorkmanRepository::Add(int _id, WorkmanType w)
{
	Workman* workman = WorkmanResolveOperation::Resolve(w);
	workman->id = _id;

	repository.push_back(workman);
	return workman;
}

#pragma endregion WorkmanRepository

#pragma region ActionFilter

ActionFilter::ActionFilter()
{
	actionFilters = nullptr;
	count = 0;

	AddPermission(WorkmanType::Assistant, ActionType::AddSubscriber);
	AddPermission(WorkmanType::Assistant, ActionType::GiveBook);

	AddPermission(WorkmanType::Manager, ActionType::BlockSubscriber);

	AddPermission(WorkmanType::Director, ActionType::BlockSubscriber);
	AddPermission(WorkmanType::Director, ActionType::DeleteSubscriber);

}

ActionFilter::Iterator ActionFilter::Find(ActionType actionType)
{
	ActionFilter x = *(this);
	for (auto i = x.begin(); i != x.end(); i++)
		if((*i).first == actionType)
			return i;
	return x.end();
}

void ActionFilter::Insert(Iterator::value_type v)
{
	if (actionFilters == nullptr)
		actionFilters = (Iterator::pointer)malloc(sizeof(std::pair<ActionType, std::pair<int, WorkmanType*>>));
	else
		actionFilters = (Iterator::pointer)realloc(actionFilters, sizeof(std::pair<ActionType, std::pair<int, WorkmanType*>>)*(count+1));

	actionFilters[count].first = v.first;
	actionFilters[count].second = v.second;

	count++;
}

void ActionFilter::AddPermission(WorkmanType workmanType, ActionType type)
{
	auto item = (*this).Find(type);
	if (item != (*this).end())
	{
		int index = (*item).second.first;
		(*item).second.second[index] = workmanType;
		(*item).second.first++;
		return;
	}

	std::pair<ActionType, std::pair<int, WorkmanType*>> tmp;
	tmp.first = type;
	tmp.second.first = 1;
	tmp.second.second = new WorkmanType[1];
	tmp.second.second[0] = workmanType;

	this->Insert(tmp);
}

WorkmanType* ActionFilter::Permissions(ActionType actionType, int* count)
{
	auto item = Find(actionType);

	if (item != end())
	{
		*count = (*item).second.first;
		return (*item).second.second;
	}

	return NULL;
}

bool ActionFilter::IsActionAvailable(WorkmanType workmanType, ActionType type)
{
	int count = 0;
	auto f = Permissions(type, &count);
	if (f == NULL)
		return false;

	for (int i = 0; i < count; i++)
		if (f[i] == workmanType)
			return true;

	return false;
}

#pragma endregion ActionFilter

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

void Library::PrintBooks(ReaderCard* card)
{
	for (auto row : card->Rows())
		std::cout << *(row->_Book()) << " " << "dateFrom: " << row->dateFrom << " DateTo: " << row->dateTo << std::endl;
}

void Library::Debts()
{
	for (auto card : readerCardRepository->Collection())
		for (auto row : card->Rows())
		{
			if (row->IsExpired())
				std::cout << *(card->_Subscriber()) << " " << *(row->_Book()) << " " << "dateFrom: " << row->dateFrom << " DateTo: " << row->dateTo << std::endl;
		}
}

ReaderCard* Library::AddSubcriber(std::string username)
{
	return readerCardRepository->Create(username);
}

/// <summary>
/// only for manager or director
/// </summary>
/// <param name="subscriber name"></param>
/// <param name="workman"></param>
void Library::SwitchSubscriberState(std::string name, Workman* workman)
{
	if(!actionFilter.IsActionAvailable(workman->type, ActionType::BlockSubscriber))
		throw PermissionDeniedException();

	ReaderCard* readerCard = readerCardRepository->Object(name);
	readerCard->SwitchIsBlocked();
}

/// <summary>
/// only for director
/// </summary>
/// <param name="name"></param>
/// <param name="workman"></param>
void Library::DeleteSubscriber(std::string name, Workman* workman)
{
	if (!actionFilter.IsActionAvailable(workman->type, ActionType::DeleteSubscriber))
		throw PermissionDeniedException();

	ReaderCard* readerCard = readerCardRepository->Object(name);
	
	readerCard->isDeleted = true;
}

void Library::GiveBook(ReaderCard* readerCard, Book* book, Workman* workman)
{
	if (!actionFilter.IsActionAvailable(workman->type, ActionType::GiveBook))
		throw PermissionDeniedException();
	
	readerCard->AddRow(book);
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

#pragma region IRepository

template<class T>
void IRepository<T>::UpdateData()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_REPOSITORIES_DATA_DELAY));
		isBusy = true;
		std::vector<T*> fromFile = Build();

		// update (memory + file)
		for (auto item : fromFile)
			if (std::find_if(repository.begin(), repository.end(),
				[&](T* obj) { return obj->id == item->id; }) == repository.end())
			{
				repository.push_back(item);
			}

		// serialize array
		std::vector< std::map<std::string, std::string>> arr;

		for (auto obj : repository)
			arr.push_back(Serialize(obj));

		std::string json = JsonHelper::SerializeArray(arr);

		std::ofstream f(filename);
		if (!f.is_open())
			continue;
		
		f << json;

		f.close();
		isBusy = false;
	}
}

template<class T>
bool IRepository<T>::IsBusy() { return isBusy; }

template<class T>
int IRepository<T>::GenerateId()
{
	int max = -1;
	for (auto s : repository)
		if (s->id > max)
			max = s->id;

	return ++max;
}

#pragma endregion IRepository

void LockAction(const std::function<void()>& f)
{
	g_lock.lock();
	f();
	g_lock.unlock();
}

void PrintActions()
{
	std::cout << "'books' - show list of books in library" << std::endl;
	std::cout << "'subs' - show list of subscribers" << std::endl;
	std::cout << "'sub_books' - show list of subscriber's books" << std::endl;
	std::cout << "'debts' - show subscriber's debts" << std::endl;
	std::cout << "'add_sub' - create subscriber's reader card" << std::endl;
	std::cout << "'give_book' - add row to the specified user reader card" << std::endl;
	std::cout << "'block_sub' - block subscriber's reader card (you can unblock it via this method too)" << std::endl;
	std::cout << "'delete_sub' - remove subscriber's reader card (you can't cancel this action)" << std::endl;
	std::cout << "'exit' - terminate programm" << std::endl;
}

int main()
{
	Library library = Library();

	auto rowRepository = RowRepository::GetInstance();
	std::thread t1(&WorkmanRepository::UpdateData, library.workmanRepository);
	 std::thread t2(&BookRepository::UpdateData, library.bookRepository);
	  std::thread t3(&SubscriberRepository::UpdateData, library.subscriberRepository);
	   std::thread t4(&RowRepository::UpdateData, rowRepository);
	    std::thread t5(&ReaderCardRepository::UpdateData, library.readerCardRepository);

	bool isBegin = true;
	while (true)
	{
		if (isBegin)
			isBegin = false;
		else
		{
			std::cout << "\nPress any key for back to menu\n";
			rewind(stdin);
			fflush(stdin);
			getchar();
			system("cls");
		}

		PrintActions();
		std::string action = "";
		std::cin >> action;
		system("cls");

		if (!action.compare("books"))
			library.PrintBooks();
		else if (!action.compare("subs"))
			library.PrintSubscribers();
		else if (!action.compare("sub_books"))
		{
			std::cout << "Input subscriber name:";
			std::string name;
			std::cin >> name;

			ReaderCard* readerCard = library.readerCardRepository->Object(name);
			if (readerCard == NULL)
			{
				std::cout << "User with specified name doesn't exists\n";
				continue;
			}

			library.PrintBooks(readerCard);
		}
		else if (!action.compare("debts"))
			library.Debts();
		else if (!action.compare("add_sub"))
		{
			std::cout << "Input subscriber name:";
			std::string name;
			std::cin >> name;

			LockAction([&]()
				{
					library.AddSubcriber(name);
				});
		}
		else if (!action.compare("give_book"))
		{
			std::cout << "Input subscriber name:";
			std::string name;
			std::cin >> name;

			ReaderCard* readerCard = library.readerCardRepository->Object(name);
			if(readerCard == NULL)
			{
				std::cout << "User is not exists\n";
				continue;
			}

			std::cout << "Input book title:";
			std::string title;
			std::cin >> title;

			Book* book = library.bookRepository->Object(title);
			if (book == NULL)
			{
				std::cout << "Book already taked or not exist\n";
				continue;
			}

			std::cout << "Input assistant id:";
			int id;
			std::cin >> id;

			Workman* assistant = library.workmanRepository->Object(id);

			LockAction([&]()
				{
					library.GiveBook(readerCard, book, assistant);
				});
		}
		else if (!action.compare("block_sub"))
		{
			std::cout << "Input subscriber name:";
			std::string name;
			std::cin >> name;

			std::cout << "Input manager or director id:";
			int id;
			std::cin >> id;
			Workman* manager = library.workmanRepository->Object(id);

			LockAction([&]()
				{
					library.SwitchSubscriberState(name, manager);
				});
		}
		else if (!action.compare("delete_sub"))
		{
			std::cout << "Input subscriber name:";
			std::string name;
			std::cin >> name;

			std::cout << "Input director id:";
			int id;
			std::cin >> id;
			Workman* director = library.workmanRepository->Object(id);

			LockAction([&]()
				{
					library.DeleteSubscriber(name, director);
				});
		}
		else if (!action.compare("exit"))
			break;

		
	}

	while (library.workmanRepository->IsBusy());
	t1.detach();

	while (library.bookRepository->IsBusy());
	t2.detach();

	while (library.subscriberRepository->IsBusy());
	t3.detach();

	while (rowRepository->IsBusy());
	t4.detach();

	while (library.readerCardRepository->IsBusy());
	t5.detach();

	return 0;
}