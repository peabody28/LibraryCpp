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

	ReaderCard* Object(Subscriber* subscriber);

	ReaderCard* Object(int subscriberId);

	std::vector<ReaderCard*> Collection();

	ReaderCard* Create(std::string username);

	void Delete(Subscriber* subscriber);

	void AddRow(Subscriber* subscriber, Book* book);
};