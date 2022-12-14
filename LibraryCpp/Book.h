#pragma once
#include <fstream>
#include <string>

class Book
{
public:
	int id;
	std::string title;
	std::string author;

	Book();

	Book(int _id, std::string _title, std::string _author);

	//Genre genre
	friend class BookRepository;
	friend std::ostream& operator << (std::ostream& os, const Book& s);
};