#pragma once
#include <vector>
#include "Row.h"

class RowRepository
{
private:
	std::vector<Row> rows;

	void Build();

public:
	RowRepository();

	Row Object(int rowId);

	void Create(Row r);

	std::vector<Row> Collection();
};
