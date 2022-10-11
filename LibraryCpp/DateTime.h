#pragma once
#include <string>
#include <iostream>
#include "StringHelper.h"

class DateTime
{
protected:
	std::string months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

public:
	int day;
	int month;
	int year;

	DateTime()
	{

	}

	DateTime(int _d, int _m, int _y)
	{
		day = _d;
		month = _m;
		year = _y;
	}
};