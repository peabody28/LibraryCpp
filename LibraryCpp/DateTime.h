#pragma once
#include <string>
#include <iostream>
#include "StringHelper.h"

class DateTime
{
public:
	int day;
	int month;
	int year;

	DateTime();

	DateTime(int _d, int _m, int _y);
};