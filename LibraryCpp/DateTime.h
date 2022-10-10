#pragma once
#include <string>

class DateTime
{
protected:
	const std::string months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

public:
	int day;
	int month;
	int year;

	DateTime(int _d, int _m, int _y)
	{
		day = _d;
		month = _m;
		year = _y;
	}

	// operator << 
};