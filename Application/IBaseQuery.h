#pragma once
#include <memory>
#include <vector>
#include <string>
#include <SQLAPI.h>
#include "ApplicationApi.h"
#include "PaginationObject.h"

template<typename T>
class APPLICATION_API IBaseQuery
{
	virtual std::shared_ptr<T> GetById(const std::string& id) = 0;
	virtual std::shared_ptr<T> GetSingle(const std::string query = "") = 0;
	virtual PaginationObject<T> GetPagination(int page, int pageSize, std::string query = "") = 0;
	virtual std::vector<std::shared_ptr<T>>GetAll(std::string query = "") = 0;
	virtual std::shared_ptr<T> GetFromCommand(SACommand& cmd) = 0;
};