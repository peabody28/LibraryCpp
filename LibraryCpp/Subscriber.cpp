#include "Subscriber.h"

int Subscriber::Id = 1;

Subscriber::Subscriber()
{
	id = Id;
}

Subscriber::Subscriber(std::string _name) { name = _name; id = Id; }