#pragma once
#include <string>
#include <vector>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include "TypeCheck.h"

template<typename T>
class DbCommandInterface
{
public:
	virtual std::string GetSingle(std::string query = "", std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) = 0;
	virtual std::string GetById(std::string Id, std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) = 0;
	virtual std::string GetMultiple(std::string query = "", std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) = 0;
	virtual std::string GetPaginated(std::string query = "", int page = 1, int page_size = 10, std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) = 0;

	virtual std::string GetSingleJson(std::string query = "", std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) = 0;
	virtual std::string GetByIdJson(std::string Id, std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) = 0;
	virtual std::string GetMultipleJson(std::string query = "", std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) = 0;
	virtual std::string GetPaginatedJson(std::string query = "", int page = 1, int page_size = 10, std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) = 0;


protected:
	virtual std::string selected_field(std::vector<std::string> select_fields = {}) = 0;
	virtual std::string include_table(std::vector<std::string>& table_list) = 0;
	virtual int field_count() = 0;

};
