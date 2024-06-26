#pragma once
#include <type_traits>
#include <BaseEntity.h>

//TODO: Implement Query class

template <typename T, typename = std::is_base_of<BaseEntity, T>::type>
class Query
{
public:
	Query();
	~Query();
	Query& Where(std::string where)
	{
		_where = where;
		return *this;
	}

	template<typename F>
	Query& Include(F f)
	{
		_includes.push_back(typeid(f()).name());
		return *this;
	}

private:
	std::string _command;
};