#pragma once
#include "ApplicationApi.h"
#include "ApplicationUser.h"
#include "IBaseQuery.h"
#include "BaseEntity.h"
#include "iostream"
#include "CoreHelper.h"
#include "ApplicationApi.h"
#include <boost/describe.hpp>
#include <boost/mp11.hpp>

template <typename T, class D = boost::describe::describe_members<T, boost::describe::mod_any_access | boost::describe::mod_inherited>>
class APPLICATION_API BaseQuery : public IBaseQuery<T>
{
public:
	BaseQuery() = default;
	explicit BaseQuery(SAConnection* con) { this->con = con; }
	virtual std::shared_ptr<T> GetById(const std::string& id) override
	{
		try
		{
			std::string table_name = typeid(T).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string query = "SELECT * FROM [dbo].[" + table_name + "] WHERE Id = :id";
			SACommand cmd(con, _TSA(query.c_str()));
			const SAString idStr(id.c_str());
			cmd.Param(_TSA("id")).setAsString() = idStr;
			cmd.Execute();
			if (cmd.FetchNext())
			{
				return GetFromCommand(cmd);
			}
		}
		catch (SAException& ex)
		{
			std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
			throw std::exception("Internal error! Database query failed");
		}
		return nullptr;
	}

	virtual std::vector<std::shared_ptr<T>>GetAll(std::string query = "") override
	{
		std::vector<std::shared_ptr<T>> items;
		try
		{
			std::string table_name = typeid(T).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string base_query = "SELECT * FROM [dbo].[" + table_name + "]";
			if (query.length() > 1)
			{
				base_query += " WHERE " + query;
			}
			SACommand cmd(con, _TSA(base_query.c_str()));
			cmd.Execute();
			while (cmd.FetchNext())
			{
				items.push_back(GetFromCommand(cmd));
			}
			return items;
		}
		catch (SAException& ex)
		{
			std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
			throw std::exception("Internal error! Database query failed");
		}
		return items;
	}

	virtual std::shared_ptr<T> GetFromCommand(SACommand& cmd) override
	{
		T* item = new T();
		boost::mp11::mp_for_each<D>([&](auto D)
		{
			if (cmd.Field(D.name).isNull())
			{
				item->*(D).pointer = "";
			}
			else if (std::is_same<decltype(item->*(D).pointer), int&>::value || typeid(item->*(D).pointer) == typeid(int))
			{
				item->*(D).pointer = cmd.Field(D.name).asLong();
			}
			else if (std::is_same<decltype(item->*(D).pointer), bool&>::value || typeid(item->*(D).pointer) == typeid(bool))
			{
				item->*(D).pointer = cmd.Field(D.name).asBool();
			}
			else if (std::is_same<decltype(item->*(D).pointer), double&>::value || typeid(item->*(D).pointer) == typeid(double)
				  || std::is_same<decltype(item->*(D).pointer), float&>::value || typeid(item->*(D).pointer) == typeid(float))
			{
				item->*(D).pointer = cmd.Field(D.name).asDouble();
			}
			else if (std::is_same<decltype(item->*(D).pointer), std::string&>::value || typeid(item->*(D).pointer) == typeid(std::string)
				|| std::is_same<decltype(item->*(D).pointer), std::wstring&>::value || typeid(item->*(D).pointer) == typeid(std::wstring)
				|| std::is_same<decltype(item->*(D).pointer), std::string_view&>::value || typeid(item->*(D).pointer) == typeid(std::string_view)
				|| std::is_same<decltype(item->*(D).pointer), std::wstring_view&>::value || typeid(item->*(D).pointer) == typeid(std::wstring_view))
			{
				item->*(D).pointer = cmd.Field(D.name).asString().GetMultiByteChars();
			}
			else if (std::is_same<decltype(item->*(D).pointer), std::tm&>::value || typeid(item->*(D).pointer) == typeid(std::tm))
			{
				item->*(D).pointer = cmd.Field(D.name).asDateTime().GetAsDBTIMESTAMP();
			}
		});
		return std::shared_ptr<T>(item);
	}
	virtual ~BaseQuery() = default;

protected:
	SAConnection* con;
};