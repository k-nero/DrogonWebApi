#pragma once
#include "DbCommandInterface.h"


template<typename T>
class MSSQLCommand : public DbCommandInterface<T>
{
public:
	MSSQLCommand() = default;
	~MSSQLCommand() = default;

	template<typename K = T>
	std::string GetSingle(std::string query = "", std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) override
	{
		throw std::exception("Not implemented");
	}

	template<typename K = T>
	std::string GetMultiple(std::string query = "", std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) override
	{
		throw std::exception("Not implemented");
	}
	template<typename K = T>
	std::string GetById(std::string query = "", std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) override
	{
		throw std::exception("Not implemented");
	}
	template<typename K = T>
	std::string GetPaginated(std::string query = "", std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) override
	{
		throw std::exception("Not implemented");
	}

	template<typename K = T>
	std::string GetSingleJson(std::string query = "", std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) override
	{
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
		std::string from = " FROM [dbo].[" + table_name + "] WHERE Id = '" + query + "' FOR JSON AUTO, WITHOUT_ARRAY_WRAPPER";
		base_query += alias + from;
		return base_query;
	}

	template<typename K = T>
	std::string GetMultipleJson(std::string query = "", std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) override
	{
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
		return base_query;
	}

	template<typename K = T>
	std::string GetByIdJson(std::string query = "", std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) override
	{
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
		std::string from = " FROM [dbo].[" + table_name + "] WHERE Id = '" + query + "' FOR JSON AUTO";
		base_query += alias + from;
		return base_query;
	}
	template<typename K = T>
	std::string GetPaginatedJson(std::string query = "", std::vector<std::string> select_fields = {}, std::vector<std::string> includes = {}) override
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
		return base_query;
	}


protected:

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

	template<typename Type = T, std::enable_if_t<std::is_class_v<Type>, bool> = true>
	std::string selected_field(std::vector<std::string> select_fields = {}) override
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
	template<typename Type = T, std::enable_if_t<std::is_class_v<Type>, bool> = true>
	std::string include_table(std::vector<std::string>& table_list) override
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
	std::string include_table(std::vector<std::string>& table_list) override
	{
		return "";
	}

};