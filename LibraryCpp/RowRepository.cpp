#include "RowRepository.h"
#include "Row.h"
#include "FileHelper.h"
#include "JsonHelper.h"
#include "DateTimeHelper.h"
#include "StringHelper.h"
#include <string>
#include <fstream>
#include <vector>

void RowRepository::Build()
{
	std::ifstream f("rows.json");

	std::string json = FileHelper::Read(&f);

	auto collection = JsonHelper::Parse(json);

	for (auto row : collection)
	{
		Row tmp = Row(StringHelper::ToInt(row["id"]), StringHelper::ToInt(row["bookId"]), StringHelper::ToInt(row["readerCardId"]), DateTimeHelper::Parse(row["dateFrom"]), DateTimeHelper::Parse(row["dateTo"]), StringHelper::ToBool(row["isReturned"]));
		rows.push_back(tmp);
	}
	f.close();

}

RowRepository::RowRepository()
{
	Build();
}

Row RowRepository::Object(int rowId)
{
	for (auto row : rows)
		if (row.id == rowId)
			return row;
}

void RowRepository::Create(Row r)
{
	rows.push_back(r);
}

std::vector<Row> RowRepository::Collection()
{
	return rows;
}

/*
std::vector<Row> RowRepository::Collection(int readerCardId) 
{ 
	std::vector<Row> collection;

	for (auto row : rows)
		if (row.readerCardId == readerCardId)
			collection.push_back(row);

	return collection;
}
*/