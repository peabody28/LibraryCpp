#pragma once
#include <map>
#include <thread>

template<class T>
class IRepository
{
private:
	bool isBusy;
protected:
	std::vector<T*> repository;

	std::string filename;

	std::vector<T*> virtual Build() = 0;

	virtual std::map<std::string, std::string> Serialize(T* obj) = 0;

public:
	void UpdateData();

	int GenerateId();

	bool IsBusy();
};