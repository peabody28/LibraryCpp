#pragma once
#include <string>
#include <iostream>
#include "StringHelper.h"

class Subscriber
{
private:
	static int Id;
	int id;
	std::string name;
public:

	Subscriber()
	{
		Id = 1;
		id = Id;
	}

	Subscriber(std::string _name) { name = _name; id = Id; }

	friend class SubscriberRepository;
	friend class ReaderCardRepository;
	friend class Library;
	friend std::ostream& operator << (std::ostream& os, const Subscriber& s);
};
