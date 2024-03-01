#pragma once
#include "ApplicationApi.h"
#include "CoreHelper.h"
#include "DbContext.h"
#include "JsonHelper.h"
#include "PaginationObject.h"
#include "TodoList.h"
#include "TypeCheck.h"
#include <any>
#include <boost/describe.hpp>
#include <boost/log/trivial.hpp>
#include <boost/mp11.hpp>
#include <future> 
#include <memory>
#include <SQLAPI.h>
#include <string>
#include <mutex>
#include <vector>

template <typename T, typename Z = std::is_base_of<BaseEntity, T>::type>
class Query
{
public:
	Query()
	{
		if (!db)
		{
			this->db = std::make_unique<DbContext>();
		}
	}
		~Query() = default;

	template<typename K = T>
	std::string FastGetById(const std::string& id, std::vector<std::string> includes = {}) noexcept(false)
	{
		std::shared_ptr<SAConnection> con(db->GetConnection());
		try
		{
			if (id.empty())
			{
				throw std::exception("Id is empty");
			}
			std::string table_name = std::string(typeid(K).name());
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string query = "SELECT *";
			std::string alias = include_table<K>(includes);
			std::string from = " FROM [dbo].[" + table_name + "] WHERE Id = '" + id + "' FOR JSON AUTO";
			query += alias + from;
			SACommand cmd(con.get(), _TSA(query.c_str()));
			cmd.Execute();
			if (cmd.FetchNext())
			{
				return std::string(cmd.Field(1).asString().GetMultiByteChars());
			}
			return "";
		}
		catch (SAException& ex)
		{
			BOOST_LOG_TRIVIAL(fatal) << ex.ErrText();
			BOOST_LOG_TRIVIAL(error) << ex.ErrMessage();
#ifdef _DEBUG
			BOOST_LOG_TRIVIAL(debug) << ex.CommandText();
			BOOST_LOG_TRIVIAL(debug) << ex.ErrNativeCode();
			BOOST_LOG_TRIVIAL(debug) << ex.ErrClass();
			BOOST_LOG_TRIVIAL(debug) << ex.ErrPos();
#endif // DEBUG
			throw std::exception(ex.ErrText().GetMultiByteChars());
		}
		return "";
	}

public:
	template<typename Type = T, std::enable_if_t<std::is_class_v<Type>, bool> = true>
	int field_count()
	{
		int count = 0;
		Type t{};
		boost::mp11::mp_for_each< boost::describe::describe_members<Type, boost::describe::mod_any_access | boost::describe::mod_inherited>>([&](auto D)
		{
			auto value = t.*(D).pointer;
			if (is_primitive_type(value))
			{
				count++;
			}
		});
	}

	template<typename Type = T, std::enable_if_t<std::is_void_v<Type>, bool> = true>
	auto field_count()
	{
		return 0;
	}



	template <typename Type = T, std::enable_if_t<std::is_class_v<Type>, bool> = true>
	std::string include_table( std::vector<std::string> table_list )
	{
		std::string table_name = std::string(typeid(Type).name());
		table_name = table_name.substr(table_name.find_last_of(' ') + 1);
		std::string alias = "";
		Type t{};
		boost::mp11::mp_for_each< boost::describe::describe_members<Type, boost::describe::mod_any_access | boost::describe::mod_inherited>>([&](auto D)
		{
			auto value = t.*(D).pointer;
			if (std::find(table_list.begin(), table_list.end(), D.name) != table_list.end())
			{
				alias += ", (SELECT *";
				using val_type = std::remove_reference_t<decltype(value)>;
				using inner_type = has_value_type_t<val_type>;
				
				std::string inner_table_name;

				table_list.erase(std::remove_if(table_list.begin(), table_list.end(), [&](std::string s) { return s == D.name; }), table_list.end());

				if (!std::is_void_v<inner_type> && !std::is_same_v<std::string, val_type> && !std::is_same_v<std::wstring, val_type>)
				{
					using inner_elem_type = std::remove_pointer_t<has_element_type_t<std::remove_reference_t<inner_type>>>;

					if (!std::is_void_v<inner_elem_type>)
					{
						inner_table_name = std::string(typeid(inner_elem_type).name());
						inner_table_name = inner_table_name.substr(inner_table_name.find_last_of(' ') + 1);
						alias += include_table<inner_elem_type>(table_list);
						alias += " FROM [dbo].[" + inner_table_name + "] WHERE [dbo].[" + table_name + "].Id = [dbo].[" + inner_table_name + "]." + table_name + "Id FOR JSON AUTO) AS '" + inner_table_name + "'";
					}
				}
				else if (is_shared_ptr_v<val_type>)
				{
					using inner_elem_type = std::remove_pointer_t<has_element_type_t<std::remove_reference_t<val_type>>>;
					if (!std::is_void_v<inner_elem_type>)
					{
						inner_table_name = std::string(typeid(inner_elem_type).name());
						inner_table_name = inner_table_name.substr(inner_table_name.find_last_of(' ') + 1);
						alias += include_table<inner_elem_type>(table_list);
						alias += " FROM [dbo].[" + inner_table_name + "] WHERE [dbo].[" + inner_table_name + "].Id = [dbo].[" + table_name + "]." + inner_table_name + "Id FOR JSON AUTO) AS '" + inner_table_name + "'";
					}
				}

			}
		});
		return alias;
	}

	template <typename Type = T, std::enable_if_t<std::is_void_v<Type>, bool> = true>
	std::string include_table(std::vector<std::string> table_list)
	{
		return "";
	}


	private:
		std::unique_ptr<DbContext> db;

};