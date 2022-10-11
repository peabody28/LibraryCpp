#pragma once
#include "DateTime.h"
#include <ctime>

static class DateTimeHelper
{
public:
	static int Compare(DateTime a, DateTime b)
	{
		if (a.year > b.year)
			return 1;
		else if (a.year < b.year)
			return 2;

		if (a.month > b.month)
			return 1;
		else if (a.month < b.month)
			return 2;

		if (a.day > b.day)
			return 1;
		else if (a.day < b.day)
			return 2;

		return 0;
	}

	static DateTime AddMonth(DateTime d)
	{
		int year = d.month == 12 ? d.year + 1 :d.year;
		int month = d.month == 12 ? 1 : d.month + 1;

		return DateTime(d.day, month, year);
	}

	static DateTime Now()
	{
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		return DateTime(now->tm_mday, now->tm_mon, now->tm_year);
	}
};