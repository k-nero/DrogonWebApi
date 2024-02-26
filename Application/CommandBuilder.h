#pragma once
#include "BaseEntity.h"
#include <type_traits>
#include <functional>

namespace CommandBuilder
{
	template<typename T>
	class DbSet
	{
	public:
		DbSet();
		~DbSet();
		DbSet& Where(std::string where)
		{
			_where = where;
			return this;
		}

		template<typename F>
		DbSet& Include(F f)
		{
			_includes.push_back(typeid(f()).name());
			return this;
		}


	private:
		std::string _command;
		std::string _where;
		std::string _table;
		std::vector<std::string> _includes;

	};
}

