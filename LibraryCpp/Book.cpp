#include "Book.h"
#include <string>

Book::Book()
{

}

Book::Book(int _id, std::string _title, std::string _author)
{
	id = _id;
	title = _title;
	author = _author;
}
