#pragma once
#include <string>

class Subscriber
{
private:
	static int Id;
	int id;
	std::string name;
public:

	Subscriber();

	Subscriber(std::string _name);

	Subscriber(int _id, std::string _name);

	friend class SubscriberRepository;
	friend class ReaderCardRepository;
	friend class Library;
	friend std::ostream& operator << (std::ostream& os, const Subscriber& s);
};
