#pragma once
#include <vector>
#include "Row.h"
#include <fstream>
#include "JsonHelper.h"
#include "FileHelper.h"

class RowRepository
{
private:
	std::vector<Row> rows;

	void Build()
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

public:
	RowRepository()
	{
		Build();
	}

	Row Object(int rowId)
	{
		for (auto row : rows)
			if (row.id == rowId)
				return row;
	}

	void Create(Row r)
	{
		rows.push_back(r);
	}

	std::vector<Row> Collection() { return rows; }


};
