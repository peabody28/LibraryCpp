#pragma once
#include <vector>
#include "SubscriberRepository.h"
#include "IRepository.h"

class ReaderCard;
class Subscriber;
class Book;

class ReaderCardRepository : public IRepository<ReaderCard>
{
private:
	SubscriberRepository* subscriberRepository;

	ReaderCardRepository();

	static ReaderCardRepository* obj;

protected:
	void Build() override;

	std::map<std::string, std::string> Serialize(ReaderCard* obj)  override;

public:
	static ReaderCardRepository* GetInstance();

	ReaderCard* Object(std::string name);

	ReaderCard* Object(int subscriberId);

	std::vector<ReaderCard*> Collection();

	bool AlreadyTaken(Book* book);

	ReaderCard* Create(std::string username);

	int GenerateReaderCardId();

	void Delete(Subscriber* subscriber);

	void AddRow(Subscriber* subscriber, Book* book);
};