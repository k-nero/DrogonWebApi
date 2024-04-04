#pragma once
#include "ApplicationApi.h"
#include "CoreHelper.h"
#include "DbContext.h"
#include "JsonHelper.h"
#include "JsonParser.h"
#include "PaginationObject.h"
#include "TypeCheck.h"
#include <any>
#include <boost/describe.hpp>
#include <boost/log/trivial.hpp>
#include <boost/mp11.hpp>
#include <future> 
#include <memory>
#include <mutex>
#include <SQLAPI.h>
#include <string>
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

	template<typename Z> [[nodiscard]]
	Z ParseFromJSON(std::string& json) noexcept(false)
	{
		return std::move(JsonParser<Z>::obj_from_json(CoreHelper::ParseJson(json)));
	}
	
	template<typename Z> [[nodiscard]]
	Z ParseFromJSON(std::string&& json) noexcept(false)
	{
		return std::move(JsonParser<Z>::obj_from_json(CoreHelper::ParseJson(std::forward<std::string&>(json))));
	}

	template<typename K = T> [[nodiscard]]
	std::shared_ptr<K> GetByIdEw(const std::string& id, std::vector<std::string> includes = {}, std::vector<std::string> select_fields = {}) noexcept(false)
	{
		auto result = GetByIdEx<K>(id, includes, select_fields);
		if(result == nullptr)
		{
			return nullptr;
		}
		auto json = CoreHelper::ParseJson(*result);
		return std::move(JsonParser<std::shared_ptr<K>>::obj_from_json(json));
	}

	template<typename K = T>
	std::shared_ptr<std::string> GetByIdEx(const std::string& id, std::vector<std::string> includes = {}, std::vector<std::string> select_fields = {}) noexcept(false)
	{
		//std::shared_ptr<SAConnection> con(db->GetConnection());
		std::shared_ptr<DbClient> client(db->GetClient());
		try
		{
			if (id.empty())
			{
				throw std::exception("Id is empty");
			}
			std::string table_name = std::string(typeid(K).name());
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string base_query = "";
			if (select_fields.empty())
			{
				base_query += "SELECT *";
			}
			else
			{
				base_query += "SELECT " + selected_field<K>(select_fields);
			}
			std::string alias = include_table<K>(includes);
			std::string from = " FROM [dbo].[" + table_name + "] WHERE Id = '" + id + "' FOR JSON AUTO, WITHOUT_ARRAY_WRAPPER";
			base_query += alias + from;
#ifdef LOG_SQL_COMMAND
			BOOST_LOG_TRIVIAL(debug) << base_query;
#endif // LOG_SQL_COMMAND
			client->CreateCommand(base_query);
			client->ExecuteCommand();
			auto result = std::make_shared<std::string>();
			result->reserve(0x800);
			client->GetJsonStringResult(result);
			if (result->empty())
			{
				return nullptr;
			}
			result->shrink_to_fit();
			return result;
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

	template<typename K = T> [[nodiscard]]
	std::shared_ptr<K> GetSingleEw(const std::string query = "", std::vector<std::string> includes = {}, std::vector<std::string> select_fields = {}) noexcept(false)
	{
		auto result = GetSingleEx<K>(query, includes, select_fields);
		if( result == nullptr)
		{
			return nullptr;
		}
		auto json = CoreHelper::ParseJson(*result);
		return std::move(JsonParser<std::shared_ptr<K>>::obj_from_json(json));
	}

	template<typename K = T>
	std::shared_ptr<std::string> GetSingleEx(const std::string query = "", std::vector<std::string> includes = {}, std::vector<std::string> select_fields = {}) noexcept(false)
	{
		try
		{
			std::shared_ptr<DbClient> client(db->GetClient());

			if (query.empty())
			{
				throw std::exception("Query is empty");
			}

			std::string table_name = std::string(typeid(K).name());
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string base_query = "";
			if (select_fields.empty())
			{
				base_query += "SELECT TOP 1 *";
			}
			else
			{
				base_query += "SELECT TOP 1 " + selected_field<K>(select_fields);
			}
			std::string alias = include_table<K>(includes);
			std::string from = " FROM [dbo].[" + table_name + "] WHERE " + query + " FOR JSON AUTO, WITHOUT_ARRAY_WRAPPER";
			base_query += alias + from;
#ifdef LOG_SQL_COMMAND
			BOOST_LOG_TRIVIAL(debug) << base_query;
#endif // LOG_SQL_COMMAND
			client->CreateCommand(base_query);
			client->ExecuteCommand();
			auto result = std::make_shared<std::string>();
			result->reserve(0x800);
			client->GetJsonStringResult(result);
			if (result->empty())
			{
				return nullptr;
			}

			result->shrink_to_fit();
			return result;
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

	template<typename K = T> [[nodiscard]]
	std::vector<std::shared_ptr<K>> GetAllEw(const std::string query = "", std::vector<std::string> includes = {}, std::vector<std::string> select_fields = {}) noexcept(false)
	{
		auto result = GetAllEx<K>(query, includes, select_fields);
		if(result == nullptr)
		{
			return std::vector<std::shared_ptr<K>>();
		}
		auto json = CoreHelper::ParseJson(*result);
		return std::move(JsonParser<std::vector<std::shared_ptr<K>>>::obj_from_json(json));
	}

	template<typename K = T>
	std::shared_ptr<std::string> GetAllEx(std::string query = "", std::vector<std::string> includes = {}, std::vector<std::string> select_fields = {}) noexcept(false)
	{
		try
		{
			std::shared_ptr<DbClient> client(db->GetClient());
			std::string table_name = std::string(typeid(K).name());
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string base_query = "";
			if (select_fields.empty())
			{
				base_query += "SELECT *";
			}
			else
			{
				base_query += "SELECT " + selected_field<K>(select_fields);
			}
			std::string alias = include_table<K>(includes);
			if (query.empty())
				query = "1=1";
			std::string from = " FROM [dbo].[" + table_name + "] WHERE " + query + " FOR JSON AUTO";
			base_query += alias + from;
#ifdef LOG_SQL_COMMAND
			BOOST_LOG_TRIVIAL(debug) << base_query;
#endif // LOG_SQL_COMMAND
			client->CreateCommand(base_query.c_str());
			client->ExecuteCommand();
			auto result = std::make_shared<std::string>();
			result->reserve(0x200000);
			client->GetJsonStringResult(result);
			if(result->empty())
			{
				return nullptr;
			}
			result->shrink_to_fit();
			return result;
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

	template<typename K = T> [[nodiscard]]
	std::shared_ptr<std::string> GetPaginatedFw(int page, int pageSize, std::string query = "", std::vector<std::string> includes = {}, std::vector<std::string> select_fields = {}) noexcept(false)
	{
		try
		{
			std::shared_ptr<DbClient> client(db->GetClient());
			std::string table_name = std::string(typeid(K).name());
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);

			auto count_task = std::async(std::launch::async, [&]()
			{
				long long count = 0;
				if (query.empty())
				{
					query += "1=1";
				}
				std::string count_query = "SELECT COUNT(*) FROM [dbo].[" + table_name + "]";
				count_query += " WHERE " + query;
				client->CreateCommand(count_query);
				client->ExecuteCommand();

				if (client->FetchNext())
				{
					count = client->GetLongResult(1);
				}

				return count;
			});

			auto result = GetPaginatedEx<K>(page, pageSize, query, includes, select_fields);
			if(result == nullptr)
			{
				return nullptr;
			}
			Json::Value root;
			root["m_data"] = CoreHelper::ParseJson(*result);
			root["m_pageSize"] = pageSize;
			root["m_currentPage"] = page;
			root["m_totalPages"] = pageSize == 0 ? 0 : ceil((double)count_task.get() / (double)pageSize);
			return std::make_shared<std::string>(root.toStyledString());
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
	template<typename K = T> [[nodiscard]]
	std::shared_ptr<PaginationObject<K>> GetPaginatedEw(int page, int pageSize, std::string query = "", std::vector<std::string> includes = {}, std::vector<std::string> select_fields = {}) noexcept(false)
	{
		std::shared_ptr<DbClient> client(db->GetClient());

		std::string table_name = std::string(typeid(K).name());
		table_name = table_name.substr(table_name.find_last_of(' ') + 1);

		auto count_task = std::async(std::launch::async, [&]()
		{
			long long count = 0;

			if (query.empty())
			{
				query += "1=1";
			}

			std::string count_query = "SELECT COUNT(*) FROM [dbo].[" + table_name + "]";
			count_query += " WHERE " + query;
			client->CreateCommand(count_query);
			client->ExecuteCommand();

			if (client->FetchNext())
			{
				count = client->GetLongResult(1);
			}

			return count;
		});

		auto result = GetPaginatedEx<K>(page, pageSize, query, includes, select_fields);
		if(result == nullptr)
		{
			return nullptr;
		}
		Json::Value root;
		root["m_data"] = CoreHelper::ParseJson(*result);
		root["m_pageSize"] = pageSize;
		root["m_currentPage"] = page;
		root["m_totalPages"] = pageSize == 0 ? 0 : ceil((double)count_task.get() / (double)pageSize);
		return std::move(JsonParser<std::shared_ptr<PaginationObject<K>>>::obj_from_json(root));
	}

	template<typename K = T>
	std::shared_ptr<std::string> GetPaginatedEx(int page, int pageSize, std::string query = "", std::vector<std::string> includes = {}, std::vector<std::string> select_fields = {}) noexcept(false)
	{
		std::shared_ptr<DbClient> client(db->GetClient());
		try
		{
			if (query.empty())
			{
				query = "1=1";
			}

			std::string table_name = std::string(typeid(K).name());
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);

			std::string fields = "";

			for (auto& include_field : includes)
			{
				fields += "[R]." + include_field + ",";
			}

			if (select_fields.empty())
			{
				fields += selected_field<K>();
			}
			else
			{
				fields += selected_field<K>(select_fields);
			}
			std::string base_query = "SELECT " + fields + " FROM (SELECT ROW_NUMBER() OVER ( ORDER BY CreatedDate ) AS RowNum";
			std::string alias = include_table<K>(includes);

			std::string from = ", * FROM [dbo].[" + table_name + "] WHERE " + query + ") AS R WHERE RowNum BETWEEN " + std::to_string((page - 1) * pageSize + 1) + " AND " + std::to_string(page * pageSize) + " ORDER BY RowNum FOR JSON AUTO";
			base_query += alias + from;
#ifdef LOG_SQL_COMMAND
			BOOST_LOG_TRIVIAL(debug) << base_query;
#endif // LOG_SQL_COMMAND

			client->CreateCommand(base_query);
			client->ExecuteCommand();
			
			auto result = std::make_shared<std::string>();
			result->reserve(page * pageSize * 0x800);
			//the json output of the result set is single collumn but split to multiple row on large data, need to concatenate them for a single json object
			client->GetJsonStringResult(result);

			if (result->empty())
			{
				return nullptr;
			}

			result->shrink_to_fit();
			return result;
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

private:

	template<typename Type = T, std::enable_if_t<std::is_class_v<Type>, bool> = true>
	std::string selected_field(std::vector<std::string> select_fields = {})
	{
		std::string fields = "";
		Type t{};
		if (select_fields.empty())
		{
			boost::mp11::mp_for_each< boost::describe::describe_members<Type, boost::describe::mod_any_access | boost::describe::mod_inherited>>([&](auto D)
			{
				auto value = t.*(D).pointer;
				if (is_primitive_type(value))
				{
					fields += "[R]." + std::string(D.name) + ",";
				}
			});
		}
		else
		{
			for (auto& field : select_fields)
			{
				fields += "[R]." + field + ",";
			}
		}
		fields.pop_back();
		return fields;
	}

	template<typename Type = T, std::enable_if_t<std::is_void_v<Type>, bool> = true>
	std::string selected_field(std::vector<std::string> select_fields = {})
	{
		return "";
	}

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
	std::string include_table(std::vector<std::string>& table_list)
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
				alias += ", JSON_QUERY ((SELECT *";
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
						alias += " FROM [dbo].[" + inner_table_name + "] WHERE [dbo].[" + table_name + "].[Id] = [dbo].[" + inner_table_name + "].[" + table_name + "Id] FOR JSON AUTO)) AS '" + std::string(D.name) + "'";
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
						alias += " FROM [dbo].[" + inner_table_name + "] WHERE [dbo].[" + inner_table_name + "].[Id] = [dbo].[" + table_name + "].[" + inner_table_name + "Id] FOR JSON AUTO, WITHOUT_ARRAY_WRAPPER)) AS '" + std::string(D.name) + "'";
					}
				}
			}
		});
		return alias;
	}

	template <typename Type = T, std::enable_if_t<std::is_void_v<Type>, bool> = true>
	std::string include_table(std::vector<std::string>& table_list)
	{
		return "";
	}

private:
	std::unique_ptr<DbContext> db;
};
