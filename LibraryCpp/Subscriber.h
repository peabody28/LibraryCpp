#pragma once
#include <string>
#include <fstream>

class Subscriber
{
private:
	int Id;
	std::string name;
public:
	int id;

	Subscriber();

	Subscriber(std::string _name);

	Subscriber(int _id, std::string _name);
};
