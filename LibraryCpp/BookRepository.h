#pragma once
#include <vector>
#include "Book.h"

class BookRepository
{
private:
	std::vector<Book> books;

	void Build();

public:
	BookRepository();

	Book Object(int id);

	std::vector<Book> Collection();
};