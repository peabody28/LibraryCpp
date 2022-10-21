#pragma once
#include <string>

class DateTime;

class DateTimeHelper
{
public:
	static int Compare(DateTime a, DateTime b);

	static DateTime AddMonth(DateTime d);

	static DateTime Now();

	static DateTime Parse(std::string s);

	static std::string ToString(DateTime& date);
};