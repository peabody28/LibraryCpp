#pragma once
#include <vector>
#include "Book.h"

class BookRepository
{
private:
	std::vector<Book*> books;

	void Build();

	BookRepository();

	static BookRepository* obj;

public:

	static BookRepository* GetInstance();

	Book* Object(int id);

	std::vector<Book*> Collection();
};