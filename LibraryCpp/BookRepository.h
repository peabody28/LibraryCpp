#pragma once
#include <vector>
#include "Book.h"

class BookRepository
{
private:
	std::vector<Book> books;
public:
	Book Object(int id)
	{
		for (auto book : books)
			if (book.id == id)
				return book;
	}
};