#pragma once
#include <vector>

class Row;

class RowRepository
{
private:
	static RowRepository* obj;
	std::vector<Row*> rows;

	void Build();
	
	RowRepository();
public:
	static void UpdateData();

	static RowRepository* GetInstance();

	Row* Object(int rowId);

	void Create(Row* r);

	std::vector<Row*> Collection(int readerCardId);
};
