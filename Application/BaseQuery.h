#pragma once
#include "ApplicationApi.h"
#include "iostream"
#include "CoreHelper.h"
#include "ApplicationApi.h"
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <boost/log/trivial.hpp>
#include "TypeCheck.h"
#include "JsonHelper.h"
#include <any>
#include "TodoList.h"
#include <memory>
#include <vector>
#include <string>
#include <SQLAPI.h>
#include "ApplicationApi.h"
#include "PaginationObject.h"

//TODO: Apply concepts
template <typename T>
class APPLICATION_API BaseQuery
{
public:
	BaseQuery() = default;
	explicit BaseQuery(SAConnection* con) { this->con = con; }
	template<typename K = T>
	std::shared_ptr<K> GetById(const std::string& id, std::vector<std::string> includes = {}) noexcept(false)
	{
		try
		{
			if (id.empty())
			{
				throw std::exception("Id is empty");
			}

			std::string table_name = typeid(K).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string query = "SELECT * FROM [dbo].[" + table_name + "] WHERE Id = :id";
			SACommand cmd(con, _TSA(query.c_str()));
			const SAString idStr(id.c_str());
			cmd.Param(_TSA("id")).setAsString() = idStr;
			cmd.Execute();
			auto item = std::make_shared<K>();
			if (cmd.FetchNext())
			{
				item = GetFromCmd(cmd);
				boost::mp11::mp_for_each< boost::describe::describe_members<K, boost::describe::mod_any_access | boost::describe::mod_inherited>>([&](auto D)
				{
					if (std::find(includes.begin(), includes.end(), D.name) != includes.end())
					{
						using type = std::remove_reference_t<decltype(item.get()->*(D).pointer)>;
						using inner_type = has_value_type_t<type>;
						if (!std::is_void_v<inner_type> && !std::is_same_v<std::string, type> && !std::is_same_v<std::wstring, type>)
						{
							using inner_elem_type = std::remove_pointer_t<has_element_type_t<std::remove_reference_t<inner_type>>>;

							if (!std::is_void_v<inner_elem_type>)
							{
								std::vector<std::shared_ptr<inner_elem_type>> inner_items;
								//TODO: Include 
								includes.erase(std::remove_if(includes.begin(), includes.end(), [&](std::string s) { return s == D.name; }), includes.end());
								inner_items = GetAll<inner_elem_type>(table_name + "Id = '" + item.get()->GetId() + "'", includes);
								(item.get()->*(D).pointer) = std::any_cast<type>(inner_items);
							}
						}
						else if (is_shared_ptr_v<type>)
						{
							using inner_elem_type = std::remove_pointer_t<has_element_type_t<std::remove_reference_t<type>>>;
							if (!std::is_void_v<inner_elem_type>)
							{
								std::string inner_table_name = typeid(inner_elem_type).name();
								inner_table_name = inner_table_name.substr(inner_table_name.find_last_of(' ') + 1);
								std::string id = std::string(cmd.Field(std::string(inner_table_name + "Id").c_str()).asString().GetMultiByteChars());
								//TODO: Include 
								includes.erase(std::remove_if(includes.begin(), includes.end(), [&](std::string s) { return s == D.name; }), includes.end());
								std::shared_ptr<inner_elem_type> inner_ptr_item = GetSingle<inner_elem_type>("Id =  '" + id + "'", includes);

								(item.get()->*(D).pointer) = std::any_cast<type>(inner_ptr_item);
							}
						}

					}

				});
			}
			return item;
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

	template<typename K = T, std::enable_if_t<std::is_void_v<K>, bool> = true>
	auto GetAll(std::string query = "", std::vector<std::string> includes = {}) noexcept(false)
	{
		return std::vector<std::shared_ptr<K>>();
	}

	template<typename K = T, std::enable_if_t<std::is_class_v<K>, bool> = true>
	std::vector<std::shared_ptr<K>>GetAll(std::string query = "", std::vector<std::string> includes = {}) noexcept(false)
	{
		std::vector<std::shared_ptr<K>> items;
		try
		{
			std::string table_name = typeid(K).name();
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
				items.push_back(GetFromCmd<K>(cmd));
			}
			return items;
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
		return items;
	}

	[[deprecated]]
	std::shared_ptr<T> GetFromCommand(SACommand& cmd, std::vector<std::string> includes = {}) noexcept(false)
	{
		T* item = new T();
		boost::mp11::mp_for_each<boost::describe::describe_members<T, boost::describe::mod_any_access | boost::describe::mod_inherited>>([&](auto D)
		{

			/*if (cmd.Field(D.name).isNull())
			{
				auto pointer = (void*)&(item->*(D).pointer);
			}*/
			if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, int>::value)
			{
				(long&)(item->*(D).pointer) = cmd.Field(D.name).asLong();
			}
			else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, bool>::value)
			{
				(bool&)(item->*(D).pointer) = cmd.Field(D.name).asBool();
			}
			else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, double>::value
				|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, float>::value)
			{
				(double&)(item->*(D).pointer) = cmd.Field(D.name).asDouble();
			}
			else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::string>::value
				|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::wstring>::value
				|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::string_view>::value
				|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::wstring_view>::value)
			{
				if (cmd.Field(D.name).isNull())
				{
					(std::string&)(item->*(D).pointer) = "null";
				}
				else
				{
					(std::string&)(item->*(D).pointer) = cmd.Field(D.name).asString().GetMultiByteChars();
				}
			}
			else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::tm>::value)
			{
				(std::tm&)(item->*(D).pointer) = std::tm(cmd.Field(D.name).asDateTime());
			}
			else
			{
				//TODO: Join table
			}
		});
		return std::shared_ptr<T>(item);
	}

	template<typename K = T, std::enable_if_t<std::is_void_v<K>, bool> = true>
	auto GetSingle(const std::string query = "", std::vector<std::string> includes = {}) noexcept(false)
	{
		return nullptr;
	}


	template<typename K = T, std::enable_if_t<std::is_class_v<K>, bool> = true>
	std::shared_ptr<K> GetSingle(const std::string query = "", std::vector<std::string> includes = {}) noexcept(false)
	{
		try
		{
			std::string table_name = typeid(K).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string base_query = "SELECT * FROM [dbo].[" + table_name + "] WHERE " + query;
			SACommand cmd(con, _TSA(base_query.c_str()));
			cmd.Execute();
			auto item = std::make_shared<K>();
			if (cmd.FetchNext())
			{
				item = GetFromCmd<K>(cmd);
				boost::mp11::mp_for_each<boost::describe::describe_members<K, boost::describe::mod_any_access | boost::describe::mod_inherited>>([&](auto D)
				{
					if (std::find(includes.begin(), includes.end(), D.name) != includes.end())
					{
						using type = std::remove_reference_t<decltype(item.get()->*(D).pointer)>;
						using inner_type = has_value_type_t<type>;
						if (!std::is_void_v<inner_type> && !std::is_same_v<std::string, type> && !std::is_same_v<std::wstring, type>)
						{
							using inner_elem_type = std::remove_pointer_t<has_element_type_t<std::remove_reference_t<inner_type>>>;

							if (!std::is_void_v<inner_elem_type>)
							{
								std::vector<std::shared_ptr<inner_elem_type>> inner_items;
								//TODO: Include 
								includes.erase(std::remove_if(includes.begin(), includes.end(), [&](std::string s) { return s == D.name; }), includes.end());
								inner_items = GetAll<inner_elem_type>(table_name + "Id = '" + item.get()->GetId() + "'", includes);
								(item.get()->*(D).pointer) = std::any_cast<type>(inner_items);
							}
						}
						else if (is_shared_ptr_v<type>)
						{
							using inner_elem_type = std::remove_pointer_t<has_element_type_t<std::remove_reference_t<type>>>;
							if (!std::is_void_v<inner_elem_type>)
							{
								std::string inner_table_name = typeid(inner_elem_type).name();
								inner_table_name = inner_table_name.substr(inner_table_name.find_last_of(' ') + 1);
								std::string id = std::string(cmd.Field(std::string(inner_table_name + "Id").c_str()).asString().GetMultiByteChars());
								//TODO: Include 
								includes.erase(std::remove_if(includes.begin(), includes.end(), [&](std::string s) { return s == D.name; }), includes.end());
								std::shared_ptr<inner_elem_type> inner_ptr_item = GetSingle<inner_elem_type>("Id =  '" + id + "'", includes);

								(item.get()->*(D).pointer) = std::any_cast<type>(inner_ptr_item);
							}

						}
					}
				});
			}
			return item;
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
	}

	template<typename K = T>
	PaginationObject<K> GetPagination(int page, int pageSize, std::string query = "") noexcept(false)
	{
		try
		{
			std::string table_name = typeid(K).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			int count = 0;
			std::string base_query = "SELECT COUNT(*) FROM [dbo].[" + table_name + "]";
			if (query.length() > 1)
			{
				base_query += " WHERE " + query;
			}
			SACommand cmd(con, _TSA(base_query.c_str()));

			cmd.Execute();
			if (cmd.FetchNext())
			{
				count = cmd.Field(1).asLong();
			}

			base_query = "SELECT * FROM [dbo].[" + table_name + "]";
			if (query.length() > 1)
			{
				base_query += " WHERE " + query;
			}

			int offset = (page - 1) * pageSize;
			base_query += " ORDER BY Id OFFSET " + std::to_string(offset) + " ROWS FETCH NEXT " + std::to_string(pageSize) + " ROWS ONLY";
			SACommand cmd2(con, _TSA(base_query.c_str()));
			cmd2.Execute();
			std::vector<std::shared_ptr<K>> items;
			while (cmd2.FetchNext())
			{
				items.push_back(GetFromCmd(cmd2));
			}
			return PaginationObject<K>(items, count, page, pageSize);
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
	}

	template<typename K = T, std::enable_if_t<std::is_void_v<K>, bool> = true>
	auto GetFromCmd(SACommand& cmd, std::vector<std::string> includes = {}) noexcept(false)
	{
		return nullptr;
	}

	template<typename K = T, std::enable_if_t<std::is_class_v<K>, bool> = true>
	std::shared_ptr<K> GetFromCmd(SACommand& cmd, std::vector<std::string> includes = {}) noexcept(false)
	{
		K* item = new K();
		boost::mp11::mp_for_each<boost::describe::describe_members<K, boost::describe::mod_any_access | boost::describe::mod_inherited>>([&](auto D)
		{

			/*if (cmd.Field(D.name).isNull())
			{
				auto pointer = (void*)&(item->*(D).pointer);
			}*/
			if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, int>::value)
			{
				(long&)(item->*(D).pointer) = cmd.Field(D.name).asLong();
			}
			else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, bool>::value)
			{
				(bool&)(item->*(D).pointer) = cmd.Field(D.name).asBool();
			}
			else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, double>::value
				|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, float>::value)
			{
				(double&)(item->*(D).pointer) = cmd.Field(D.name).asDouble();
			}
			else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::string>::value
				|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::wstring>::value
				|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::string_view>::value
				|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::wstring_view>::value)
			{
				if (cmd.Field(D.name).isNull())
				{
					(std::string&)(item->*(D).pointer) = "null";
				}
				else
				{
					(std::string&)(item->*(D).pointer) = cmd.Field(D.name).asString().GetMultiByteChars();
				}
			}
			else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::tm>::value)
			{
				(std::tm&)(item->*(D).pointer) = std::tm(cmd.Field(D.name).asDateTime());
			}
			else
			{
				//TODO: Join table
			}
		});
		return std::shared_ptr<K>(item);
	}

	virtual ~BaseQuery() = default;

protected:
	SAConnection* con = nullptr;
};
