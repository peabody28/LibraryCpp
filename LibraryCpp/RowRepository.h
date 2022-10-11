#pragma once
#include <vector>
#include "Row.h"

class RowRepository
{
private:
	std::vector<Row> rows;
public:

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
