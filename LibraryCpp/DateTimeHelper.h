#pragma once
#include "DateTime.h"
#include <string>

class DateTimeHelper
{
public:
	static int Compare(DateTime a, DateTime b);

	static DateTime AddMonth(DateTime d);

	static DateTime Now();

	static DateTime Parse(std::string s);
};