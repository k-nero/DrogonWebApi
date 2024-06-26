#pragma once
#include "ApplicationApi.h"
#include <boost/describe.hpp>
#include <boost/log/trivial.hpp>
#include <boost/mp11.hpp>
#include <iostream>
#include "DbContext.h"
#include "CoreHelper.h"
#include "TypeCheck.h"


template <typename T, class D = boost::describe::describe_members<T, boost::describe::mod_any_access | boost::describe::mod_inherited>>
class APPLICATION_API BaseCommand
{
public:
	BaseCommand()
	{
		if (!db)
		{
			this->db = std::make_unique<DbContext>();
		}
	}
	explicit BaseCommand(DbContext* db) { this->db = std::make_unique<DbContext>(db); }
	virtual int Create(T& item) noexcept(false)
	{
		std::shared_ptr<DbClient> client(db->GetClient());
		try
		{
			std::string table_name = typeid(T).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string query = "INSERT INTO [dbo].[" + table_name + "] (";
			std::string values = " VALUES (";
			std::vector<std::string> fields;
			boost::mp11::mp_for_each<D>([&](auto D)
			{
				std::string field = D.name;
				auto value = (item.*(D).pointer);
				auto void_pointer = (void*)&value;
				if (is_primitive_type(value))
				{
					if constexpr (D.name == "CreatedDate")
					{
						values += "GETDATE(), ";
					}
					else
					{
						if constexpr (std::is_same<std::remove_reference_t<decltype(value)>, std::string>::value
							|| std::is_same<std::remove_reference_t<decltype(value)>, std::wstring>::value
							|| std::is_same<std::remove_reference_t<decltype(value)>, std::string_view>::value
							|| std::is_same<std::remove_reference_t<decltype(value)>, std::wstring_view>::value)
						{
							if ((*(std::string*)void_pointer).empty())
							{
								return;
							}
						}
						values += ":" + field + ", ";
						fields.push_back(field);
					}
					query += field + ", ";
				}
			});
			query = query.substr(0, query.length() - 2) + ")";
			values = values.substr(0, values.length() - 2) + ")";
			query += values;
			client->CreateCommand(query);
			boost::mp11::mp_for_each<D>([&](auto D)
			{
				for (auto& f : fields)
				{
					if (D.name == f)
					{
						auto value = (item.*(D).pointer);
						auto void_pointer = (void*)&value;
						if constexpr (std::is_same<std::remove_reference_t<decltype(value)>, int>::value)
						{
							client->BindParameter(D.name, *(int*)void_pointer);
						}
						else if constexpr (std::is_same<std::remove_reference_t<decltype(value)>, long>::value)
						{
							
							client->BindParameter(D.name, *(long*)void_pointer);
						}
						else if constexpr (std::is_same<std::remove_reference_t<decltype(value)>, bool>::value)
						{
							client->BindParameter(D.name, *(bool*)void_pointer);
						}
						else if constexpr (std::is_same<std::remove_reference_t<decltype(value)>, double>::value
							|| std::is_same<std::remove_reference_t<decltype(value)>, float>::value)
						{
							client->BindParameter(D.name, *(double*)void_pointer);
						}
						else if constexpr (std::is_same<std::remove_reference_t<decltype(value)>, std::string>::value
							|| std::is_same<std::remove_reference_t<decltype(value)>, std::wstring>::value
							|| std::is_same<std::remove_reference_t<decltype(value)>, std::string_view>::value
							|| std::is_same<std::remove_reference_t<decltype(value)>, std::wstring_view>::value)
						{
							client->BindParameter(D.name, (*(std::string*)void_pointer));
						}
						else if constexpr (std::is_same<std::remove_reference_t<decltype(value)>, std::tm>::value)
						{
							client->BindParameter(D.name, *(tm*)void_pointer);
						}
					}
				}
			});
#ifdef LOG_SQL_COMMAND
			BOOST_LOG_TRIVIAL(debug) << query;
#endif // LOG_SQL_COMMAND
			client->ExecuteCommand();
			return (int)client->AffectedRows();
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
		return 0;
	}

	virtual int Update(T& item, const std::string& query) noexcept(false)
	{
		std::shared_ptr<DbClient> client(db->GetClient());
		try
		{
			std::string table_name = typeid(T).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string command = "UPDATE [dbo].[" + table_name + "] SET ";
			std::vector<std::string> fields;
			boost::mp11::mp_for_each<D>([&](auto D)
			{
				std::string field = D.name;
				if (field != "Id" && field != "CreatedDate" && is_primitive_type((item.*(D).pointer)))
				{
					if (field == "ModifiedDate")
					{
						command += field + " = GETDATE(), ";
					}
					else
					{
						command += field + " = :" + field + ", ";
						fields.push_back(field);
					}
				}
			});
			command = command.substr(0, command.length() - 2) + " WHERE " + query;
			client->CreateCommand(command);
			boost::mp11::mp_for_each<D>([&](auto D)
			{
				for (auto& f : fields)
				{
					if (D.name == f)
					{
						auto value = (item.*(D).pointer);
						auto void_pointer = (void*)&value;
						if constexpr (std::is_same<std::remove_reference_t<decltype(item.*(D).pointer)>, int>::value)
						{
							client->BindParameter(D.name, *(int*)void_pointer);
						}
						else if constexpr (std::is_same<std::remove_reference_t<decltype(item.*(D).pointer)>, long>::value)
						{
							client->BindParameter(D.name, *(long*)void_pointer);
						}
						else if constexpr (std::is_same<std::remove_reference_t<decltype(item.*(D).pointer)>, bool>::value)
						{
							client->BindParameter(D.name, *(bool*)void_pointer);
						}
						else if constexpr (std::is_same<std::remove_reference_t<decltype(item.*(D).pointer)>, double>::value
							|| std::is_same<std::remove_reference_t<decltype(item.*(D).pointer)>, float>::value)
						{
							client->BindParameter(D.name, *(double*)void_pointer);
						}
						else if constexpr (std::is_same<std::remove_reference_t<decltype(item.*(D).pointer)>, std::string>::value
							|| std::is_same<std::remove_reference_t<decltype(item.*(D).pointer)>, std::wstring>::value
							|| std::is_same<std::remove_reference_t<decltype(item.*(D).pointer)>, std::string_view>::value
							|| std::is_same<std::remove_reference_t<decltype(item.*(D).pointer)>, std::wstring_view>::value)
						{
							client->BindParameter(D.name, (*(std::string*)void_pointer));
						}
						else if constexpr (std::is_same<std::remove_reference_t<decltype(item.*(D).pointer)>, std::tm>::value)
						{
							client->BindParameter(D.name, *(tm*)void_pointer);
						}
					}
				}
			});
#ifdef LOG_SQL_COMMAND
			BOOST_LOG_TRIVIAL(debug) << command;
#endif // LOG_SQL_COMMAND
			client->ExecuteCommand();
			return (int)client->AffectedRows();
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
		return 0;
	}

	virtual int Delete(const std::string& query) noexcept(false)
	{
		std::shared_ptr<DbClient> client(db->GetClient());
		try
		{

			std::string table_name = typeid(T).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string command = "DELETE FROM [dbo].[" + table_name + "] WHERE " + query;
			client->CreateCommand(command);
#ifdef LOG_SQL_COMMAND
			BOOST_LOG_TRIVIAL(debug) << command;
#endif // LOG_SQL_COMMAND
			client->ExecuteCommand();
			return (int)client->AffectedRows();
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

	virtual ~BaseCommand() = default;
protected:
	std::unique_ptr<DbContext> db = nullptr;

};
