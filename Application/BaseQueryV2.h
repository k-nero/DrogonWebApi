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
	Query() = default;
	~Query() = default;

	template<typename K = T>
	std::shared_ptr<K> GetById(const std::string& id, std::vector<std::string> includes = {}) noexcept(false)
	{
		std::shared_ptr<SAConnection> con(db->GetConnection());
		try
		{
			if (id.empty())
			{
				throw std::exception("Id is empty");
			}
			std::string table_name = std::string(typeid(K).name()).substr(table_name.find_last_of(' ') + 1);
			std::string query = "SELECT ";

			std::vector<std::string> tables = { table_name };
			tables.insert(tables.end(), includes.begin(), includes.end());
			std::reverse(tables.begin(), tables.end());


			SACommand cmd(con.get(), _TSA(query.c_str()));
			const SAString idStr(id.c_str());
			cmd.Param(_TSA("id")).setAsString() = idStr;
			cmd.Execute();
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
		return nullptr;
	}

private:
	template<typename Type, std::enable_if_t<std::is_class_v<Type>, bool> = true>
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

	template<typename Type, std::enable_if_t<std::is_void_v<Type>, bool> = true>
	auto field_count()
	{
		return 0;
	}



	template <typename Type, std::enable_if_t<std::is_class_v<Type>, bool> = true>
	std::string table_alias_builder( std::vector<std::string> table_list )
	{
		std::string alias = "";
		Type t{};
		boost::mp11::mp_for_each< boost::describe::describe_members<Type, boost::describe::mod_any_access | boost::describe::mod_inherited>>([&](auto D)
		{
			auto value = t.*(D).pointer;
			if (is_primitive_type(value))
			{
				alias += "[" + table_list.back() + "]" + "." + (D).name + " AS " + table_list.back() + "_" + (D).name + ", ";
			}
			else if (std::find(table_list.begin(), table_list.end(), D.name) != table_list.end())
			{
				using type = std::remove_reference_t<decltype(item.get()->*(D).pointer)>;
				using inner_type = has_value_type_t<type>;

				if (!std::is_void_v<inner_type> && !std::is_same_v<std::string, type> && !std::is_same_v<std::wstring, type>)
				{
					using inner_elem_type = std::remove_pointer_t<has_element_type_t<std::remove_reference_t<inner_type>>>;

					if (!std::is_void_v<inner_elem_type>)
					{

					}
				}
				else if (is_shared_ptr_v<type>)
				{
					using inner_elem_type = std::remove_pointer_t<has_element_type_t<std::remove_reference_t<type>>>;
					if (!std::is_void_v<inner_elem_type>)
					{

					}
				}

			}
		});
	}

	template <typename Type, std::enable_if_t<std::is_void_v<Type>, bool> = true>
	auto table_alias_builder(std::vector<std::string> table_list)
	{
		return 0;
	}

};