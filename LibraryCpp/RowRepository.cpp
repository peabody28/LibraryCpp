#include "RowRepository.h"
#include "FileHelper.h"
#include "JsonHelper.h"
#include <fstream>
#include "DateTimeHelper.h"

void RowRepository::Build()
{
	std::ifstream f("rows.json");

	std::string json = FileHelper::Read(&f);

	auto collection = JsonHelper::Parse(json);

	for (auto row : collection)
	{
		Row tmp = Row(StringHelper::ToInt(row["id"]), StringHelper::ToInt(row["bookId"]), DateTimeHelper::Parse(row["dateFrom"]), DateTimeHelper::Parse(row["dateTo"]), StringHelper::ToBool(row["isReturned"]));
		rows.push_back(tmp);
	}
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

std::vector<Row> RowRepository::Collection() { return rows; }