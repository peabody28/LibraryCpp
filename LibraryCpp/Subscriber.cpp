#include "Subscriber.h"
#include <string>

Subscriber::Subscriber()
{
	Id = 0;
	id = Id;
}

Subscriber::Subscriber(std::string _name) { name = _name; id = Id; }

Subscriber::Subscriber(int _id, std::string _name) { name = _name; id = _id; }