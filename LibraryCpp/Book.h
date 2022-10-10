#pragma once

class Book
{
private:
	int id;
public:
	//Genre genre;
	friend class BookRepository;
};