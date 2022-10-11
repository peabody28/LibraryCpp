#pragma once
#include <iostream>
#include "StringHelper.h"

class Book
{
private:
	int id;
	std::string title;
	std::string author;
public:
	//Genre genre
	friend class BookRepository;
	friend std::ostream& operator << (std::ostream& os, const Book& s);
};