#pragma once
#include <vector>
#include <map>
#include "Book.h"
#include "IRepository.h"

class BookRepository : public IRepository<Book>
{
private:
	BookRepository();

	static BookRepository* obj;

protected:
	void Build() override;
	
	std::map<std::string, std::string> Serialize(Book* obj)  override;
	
public:
	static BookRepository* GetInstance();

	Book* Object(int id);

	std::vector<Book*> Collection();
};