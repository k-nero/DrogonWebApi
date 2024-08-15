#pragma once
#include <type_traits>
#include <BaseEntity.h>
#include <vector>

//TODO: Implement Query class

template <typename T, typename = std::is_base_of<BaseEntity, T>::type>
class DbSet
{
public:
	DbSet() = default;
	~DbSet() = default;

	DbSet& Where(std::string where)
	{
		_where = where;
		return *this;
	}

	template<typename F>
	DbSet& Include(F f)
	{
		_includes.push_back(typeid(f()).name());
		return *this;
	}

private:
	std::string _where;
	std::vector<std::string> _includes;
};