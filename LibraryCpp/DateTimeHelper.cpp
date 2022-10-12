#include "DateTimeHelper.h"
#include "DateTime.h"
#include <ctime>
#include <string>

int DateTimeHelper::Compare(DateTime a, DateTime b)
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

DateTime DateTimeHelper::AddMonth(DateTime d)
{
	int year = d.month == 12 ? d.year + 1 : d.year;
	int month = d.month == 12 ? 1 : d.month + 1;

	return DateTime(d.day, month, year);
}

DateTime DateTimeHelper::Now()
{
	std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);
	return DateTime(now->tm_mday, now->tm_mon, now->tm_year);
}

DateTime DateTimeHelper::Parse(std::string s)
{
	int datetime[3] = { 0,0,0 };
	int i = 0;

	int a = 0;
	auto current = s.begin();
	while (true)
	{
		if (current == s.end() || *current == '.')
		{
			datetime[i] = a;
			if (current == s.end())
				break;

			a = 0;
			i++;
			current++;
			continue;
		}

		a = a * 10 + *current - '0';

		current++;
	}

	return DateTime(datetime[0], datetime[1], datetime[2]);

}