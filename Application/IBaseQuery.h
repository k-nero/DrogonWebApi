#pragma once
#include <memory>
#include <vector>
#include <string>
#include <SQLAPI.h>
#include "ApplicationApi.h"

template<typename T>
class APPLICATION_API IBaseQuery
{
	virtual std::shared_ptr<T> GetById(const std::string& id) = 0;
	virtual std::vector<std::shared_ptr<T>>GetAll(std::string query = "") = 0;
	virtual int Create(T* item) = 0;
	//virtual int Update(T* item) = 0;
	virtual std::shared_ptr<T> GetFromCommand(SACommand& cmd) = 0;
};