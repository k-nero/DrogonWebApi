#pragma once
#include "JsonHelper.h"

#include "ApplicationApi.h"
#include "CoreHelper.h"
#include "DbContext.h"
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
#include "BaseQueryV2.h"

//TODO: Apply concepts
template <typename T, typename Z = std::is_base_of<BaseEntity, T>::type>
class APPLICATION_API BaseQuery
{
public:
	BaseQuery()
	{
		if (!db)
		{
			this->db = std::make_unique<DbContext>();
		}
	}
	explicit BaseQuery(DbContext* db) { this->db = std::make_unique<DbContext>(db); }

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
			std::string table_name = typeid(K).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string query = "SELECT * FROM [dbo].[" + table_name + "] WHERE " PKEY " = :id";
			SACommand cmd(con.get(), _TSA(query.c_str()));
			const SAString idStr(id.c_str());
			cmd.Param(_TSA("id")).setAsString() = idStr;
#ifdef LOG_SQL_COMMAND
			BOOST_LOG_TRIVIAL(debug) << query;
#endif // LOG_SQL_COMMAND
			cmd.Execute();
			auto item = std::make_shared<K>();
			if (cmd.FetchNext())
			{
				item = GetFromCmd<K>(cmd);
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
								auto includes_copy = includes;
								includes_copy.erase(std::remove_if(includes_copy.begin(), includes_copy.end(), [&](std::string s) { return s == D.name; }), includes_copy.end());
#ifdef ASYNC
								auto inner_items_future = std::async(std::launch::async, &BaseQuery::GetAll<inner_elem_type>, this, table_name + "Id = '" + std::string(cmd.Field(PKEY).asString().GetMultiByteChars()) + "'", includes);
								auto inner_items = inner_items_future.get();
#else
								auto inner_items = GetAll<inner_elem_type>(table_name + "Id = '" + std::string(cmd.Field(PKEY).asString().GetMultiByteChars()) + "'", includes);
#endif // ASYNC
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
								auto id = std::string(cmd.Field(std::string(inner_table_name + PKEY).c_str()).asString().GetMultiByteChars());
								auto includes_copy = includes;
								includes_copy.erase(std::remove_if(includes_copy.begin(), includes_copy.end(), [&](std::string s) { return s == D.name; }), includes_copy.end());
#ifdef ASYNC
								auto inner_item_future = std::async(std::launch::async, &BaseQuery::GetSingle<inner_elem_type>, this, PKEY " =  '" + id + "'", includes);
								auto inner_item = inner_item_future.get();
#else
								auto inner_item = GetSingle<inner_elem_type>(PKEY " =  '" + id + "'", includes);
#endif // ASYNC
								(item.get()->*(D).pointer) = std::any_cast<type>(inner_item);
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
	std::vector<std::shared_ptr<K>> GetAll(std::string query = "", std::vector<std::string> includes = {}) noexcept(false)
	{
		std::shared_ptr<SAConnection> con(db->GetConnection());
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
			SACommand cmd(con.get(), _TSA(base_query.c_str()));
#ifdef LOG_SQL_COMMAND
			BOOST_LOG_TRIVIAL(debug) << base_query;
#endif // LOG_SQL_COMMAND
			cmd.Execute();
			while (cmd.FetchNext())
			{
				auto item = GetFromCmd<K>(cmd);
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
								auto includes_copy = includes;
								includes_copy.erase(std::remove_if(includes_copy.begin(), includes_copy.end(), [&](std::string s) { return s == D.name; }), includes_copy.end());
#ifdef ASYNC
								auto inner_items_future = std::async(std::launch::async, &BaseQuery::GetAll<inner_elem_type>, this, table_name + "Id = '" + std::string(cmd.Field(PKEY).asString().GetMultiByteChars()) + "'", includes);
								auto inner_items(inner_items_future.get());
#else
								auto inner_items = GetAll<inner_elem_type>(table_name + "Id = '" + std::string(cmd.Field(PKEY).asString().GetMultiByteChars()) + "'", includes);
#endif // ASYNC
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
								auto id = std::string(cmd.Field(std::string(inner_table_name + "Id").c_str()).asString().GetMultiByteChars());
								auto includes_copy = includes;
								includes_copy.erase(std::remove_if(includes_copy.begin(), includes_copy.end(), [&](std::string s) { return s == D.name; }), includes_copy.end());
#ifdef ASYNC
								auto inner_item_future = std::async(std::launch::async, &BaseQuery::GetSingle<inner_elem_type>, this, PKEY " =  '" + id + "'", includes);
								auto inner_item = inner_item_future.get();
#else
								auto inner_item = GetSingle<inner_elem_type>(PKEY " =  '" + id + "'", includes);
#endif // ASYNC
								(item.get()->*(D).pointer) = std::any_cast<type>(inner_item);
							}
						}

					}

				});
				items.push_back(item);
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
			}
		});
		return std::shared_ptr<T>(item);
	}

	template<typename K = T, std::enable_if_t<std::is_void_v<K>, bool> = true>
	auto GetSingle(const std::string query = "", std::vector<std::string> includes = {}) noexcept(false)
	{
		return std::shared_ptr<K>();
	}

	template<typename K = T, std::enable_if_t<std::is_class_v<K>, bool> = true>
	std::shared_ptr<K> GetSingle(const std::string query = "", std::vector<std::string> includes = {}) noexcept(false)
	{
		std::shared_ptr<SAConnection> con(db->GetConnection());
		try
		{
			std::string table_name = typeid(K).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string base_query = "SELECT * FROM [dbo].[" + table_name + "] WHERE " + query;
			SACommand cmd(con.get(), _TSA(base_query.c_str()));
#ifdef LOG_SQL_COMMAND
			BOOST_LOG_TRIVIAL(debug) << base_query;
#endif // LOG_SQL_COMMAND
			cmd.Execute();

			std::shared_ptr<K> item;
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
								
								auto includes_copy = includes;
								includes_copy.erase(std::remove_if(includes_copy.begin(), includes_copy.end(), [&](std::string s) { return s == D.name; }), includes_copy.end());
#ifdef ASYNC
								auto inner_items_future = std::async(std::launch::async, &BaseQuery::GetAll<inner_elem_type>, this, table_name +  "Id = '" + std::string(cmd.Field(PKEY).asString().GetMultiByteChars()) + "'", includes);
								auto inner_items = inner_items_future.get();
#else 
								auto inner_items = GetAll<inner_elem_type>(table_name + "Id = '" + std::string(cmd.Field(PKEY).asString().GetMultiByteChars()) + "'", includes);
#endif	// ASYNC
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
								auto id = std::string(cmd.Field(std::string(inner_table_name + "Id").c_str()).asString().GetMultiByteChars());
								auto includes_copy = includes;
								includes_copy.erase(std::remove_if(includes_copy.begin(), includes_copy.end(), [&](std::string s) { return s == D.name; }), includes_copy.end());
#ifdef ASYNC
								auto inner_ptr_item_future = std::async(std::launch::async, &BaseQuery::GetSingle<inner_elem_type>, this, PKEY " =  '" + id + "'", includes);
								auto inner_ptr_item = inner_ptr_item_future.get();
#else 
								auto inner_ptr_item = GetSingle<inner_elem_type>(PKEY " =  '" + id + "'", includes);
#endif	// ASYNC
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
	std::shared_ptr<PaginationObject<K>> GetPagination(int page, int pageSize, std::string query = "", std::vector<std::string> includes = {}) noexcept(false)
	{
		std::shared_ptr<SAConnection> con(db->GetConnection());
		std::shared_ptr<SAConnection> cout_con(db->GetConnection());
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
			SACommand cmd(cout_con.get(), _TSA(base_query.c_str()));
			auto cout_task = std::async(std::launch::async, [&cmd]() { cmd.Execute(); });

			base_query = "SELECT * FROM ( SELECT ROW_NUMBER() OVER (ORDER BY CreatedDate) AS RowNum, * FROM [dbo].[" + table_name + "]";

			if (query.length() > 1)
			{
				base_query += " WHERE " + query;
			}

			base_query += " ) AS S WHERE RowNum BETWEEN " + std::to_string((page - 1) * pageSize + 1) + " AND " + std::to_string(page * pageSize) + " ORDER BY RowNum";

			SACommand cmd2(con.get(), _TSA(base_query.c_str()));
#ifdef LOG_SQL_COMMAND
			BOOST_LOG_TRIVIAL(debug) << base_query;
#endif // LOG_SQL_COMMAND
			cmd2.Execute();
			std::vector<std::shared_ptr<K>> items;
			while (cmd2.FetchNext())
			{
				auto item = GetFromCmd<K>(cmd2);
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
								auto includes_copy = includes;
								includes_copy.erase(std::remove_if(includes_copy.begin(), includes_copy.end(), [&](std::string s) { return s == D.name; }), includes_copy.end());
#ifdef ASYNC
								auto inner_items_future = std::async(std::launch::async, &BaseQuery::GetAll<inner_elem_type>, this, table_name + "Id = '" + std::string(cmd2.Field(PKEY).asString().GetMultiByteChars()) + "'", includes_copy);
								auto inner_items = inner_items_future.get();
#else
								auto inner_items = GetAll<inner_elem_type>(table_name + "Id = '" + std::string(cmd2.Field(PKEY).asString().GetMultiByteChars()) + "'", includes);
#endif // ASYNC
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
								auto id = std::string(cmd2.Field(std::string(inner_table_name + "Id").c_str()).asString().GetMultiByteChars());
								auto includes_copy = includes;
								includes_copy.erase(std::remove_if(includes_copy.begin(), includes_copy.end(), [&](std::string s) { return s == D.name; }), includes_copy.end());
#ifdef ASYNC
								auto inner_item_future = std::async(std::launch::async, &BaseQuery::GetSingle<inner_elem_type>, this, PKEY " =  '" + id + "'", includes);
								auto inner_item = inner_item_future.get();
#else
								auto inner_item = GetSingle<inner_elem_type>(PKEY " =  '" + id + "'", includes);
#endif // ASYNC
								(item.get()->*(D).pointer) = std::any_cast<type>(inner_item);
							}
						}

					}

				});
				items.push_back(item);
			}

			cout_task.get();
			if (cmd.FetchNext())
			{
				count = cmd.Field(1).asLong();
			}

			return std::make_shared<PaginationObject<K>>(items, pageSize, page, pageSize == 0 ? 0 : ceil(count / pageSize));
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
	std::recursive_mutex m;
	std::unique_ptr<DbContext> db = nullptr;
};