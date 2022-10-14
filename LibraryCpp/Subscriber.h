#pragma once
#include <string>
#include <fstream>

class Subscriber
{
public:
	int id;
	std::string name;

	Subscriber();

	Subscriber(std::string _name);

	Subscriber(int _id, std::string _name);
};
