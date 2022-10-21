#pragma once
#include <vector>
#include "IRepository.h"

class Row;

class RowRepository : public IRepository<Row>
{
private:
	static RowRepository* obj;
	
	RowRepository();

protected:
	void Build() override;

	std::map<std::string, std::string> Serialize(Row* obj) override;

public:
	static RowRepository* GetInstance();

	Row* Object(int rowId);

	void Create(Row* r);

	std::vector<Row*> Collection(int readerCardId);
};
