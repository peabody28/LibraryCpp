#include "BookRepository.h"
#include "StringHelper.h"
#include "FileHelper.h"
#include "JsonHelper.h"
#include "Book.h"
#include <fstream>
#include <string>
#include <vector>

void BookRepository::Build()
{
	std::ifstream f("books.json");

	std::string json = FileHelper::Read(&f);

	auto collection = JsonHelper::Parse(json);

	for (auto row : collection)
	{
		Book tmp = Book(StringHelper::ToInt(row["id"]), row["title"], row["author"]);
		books.push_back(tmp);
	}

	f.close();
}

BookRepository::BookRepository()
{
	Build();
}

Book BookRepository::Object(int id)
{
	for (auto book : books)
		if (book.id == id)
			return book;
}

std::vector<Book> BookRepository::Collection()
{
	return books;
}