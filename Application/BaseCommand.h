#pragma once
#include "ApplicationApi.h"
#include "IBaseCommand.h"
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <iostream>

template <typename T, class D = boost::describe::describe_members<T, boost::describe::mod_any_access | boost::describe::mod_inherited>>
class APPLICATION_API BaseCommand : public IBaseCommand<T>
{
public:
	BaseCommand() = default;
	explicit BaseCommand(SAConnection* con) { this->con = con; }
	virtual int Create(T* item) override
	{
		try
		{
			std::string table_name = typeid(T).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string query = "INSERT INTO [dbo].[" + table_name + "] (";
			std::string values = " VALUES (";
			SACommand cmd(con);
			cmd.setCommandText(_TSA(query.c_str()));
			std::vector<std::string> fields;
			boost::mp11::mp_for_each<D>([&](auto D)
			{
				std::string field = D.name;
				if (D.name != "ModifiedDate")
				{
					query += field + ", ";
					if (D.name == "CreatedDate")
					{
						values += "GETDATE(), ";
					}
					else
					{
						values += ":" + field + ", ";
						fields.push_back(field);
					}
				}
			});
			query = query.substr(0, query.length() - 2) + ")";
			values = values.substr(0, values.length() - 2) + ")";
			query += values;
			cmd.setCommandText(_TSA((query).c_str()));
			boost::mp11::mp_for_each<D>([&](auto D)
			{
				for (auto& f : fields)
				{
					if (D.name == f)
					{
						auto value = (item->*(D).pointer);
						auto void_pointer = (void*)&value;
						if (std::is_same<decltype(item->*(D).pointer), int&>::value || typeid(item->*(D).pointer) == typeid(int))
						{
							cmd.Param(D.name).setAsLong() = *(int*)void_pointer;
						}
						else if (std::is_same<decltype(item->*(D).pointer), long&>::value || typeid(item->*(D).pointer) == typeid(long))
						{
							cmd.Param(D.name).setAsLong() = *(long*)void_pointer;
						}
						else if (std::is_same<decltype(item->*(D).pointer), bool&>::value || typeid(item->*(D).pointer) == typeid(bool))
						{
							cmd.Param(D.name).setAsBool() = *(bool*)void_pointer;
						}
						else if (std::is_same<decltype(item->*(D).pointer), double&>::value || typeid(item->*(D).pointer) == typeid(double)
							|| std::is_same<decltype(item->*(D).pointer), float&>::value || typeid(item->*(D).pointer) == typeid(float))
						{
							cmd.Param(D.name).setAsDouble() = *(double*)void_pointer;
						}
						else if (std::is_same<decltype(item->*(D).pointer), std::string&>::value || typeid(item->*(D).pointer) == typeid(std::string)
							|| std::is_same<decltype(item->*(D).pointer), std::wstring&>::value || typeid(item->*(D).pointer) == typeid(std::wstring)
							|| std::is_same<decltype(item->*(D).pointer), std::string_view&>::value || typeid(item->*(D).pointer) == typeid(std::string_view)
							|| std::is_same<decltype(item->*(D).pointer), std::wstring_view&>::value || typeid(item->*(D).pointer) == typeid(std::wstring_view))
						{
							cmd.Param(_TSA(D.name)).setAsString() = (*(std::string*)void_pointer).c_str();
						}
						else if (std::is_same<decltype(item->*(D).pointer), std::tm&>::value || typeid(item->*(D).pointer) == typeid(std::tm))
						{
							cmd.Param(_TSA(D.name)).setAsDateTime() = SADateTime(*(tm*)void_pointer);
						}
					}
				}
			});
			cmd.Execute();
			return cmd.RowsAffected();
		}
		catch (SAException& ex)
		{
			std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
			throw std::exception("Internal error! Database insert command failed");
		}
		return 0;
	}

	virtual int Update(T* item) override
	{
		try
		{
			std::string table_name = typeid(T).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string query = "UPDATE [dbo].[" + table_name + "] SET ";
			SACommand cmd(con);
			cmd.setCommandText(_TSA(query.c_str()));
			std::vector<std::string> fields;
			boost::mp11::mp_for_each<D>([&](auto D)
			{
				std::string field = D.name;
				if (field != "Id" && field != "CreatedDate")
				{
					if (field == "ModifiedDate")
					{
						query += field + " = GETDATE(), ";
					}
					else
					{
						query += field + " = :" + field + ", ";
						fields.push_back(field);
					}
				}
			});
			query = query.substr(0, query.length() - 2) + " WHERE Id = :id";
			cmd.setCommandText(_TSA(query.c_str()));
			boost::mp11::mp_for_each<D>([&](auto D)
			{
				for (auto& f : fields)
				{
					if (D.name == f)
					{
						auto value = (item->*(D).pointer);
						auto void_pointer = (void*)&value;
						if (std::is_same<decltype(item->*(D).pointer), int&>::value || typeid(item->*(D).pointer) == typeid(int))
						{
							cmd.Param(D.name).setAsLong() = *(int*)void_pointer;
						}
						else if (std::is_same<decltype(item->*(D).pointer), long&>::value || typeid(item->*(D).pointer) == typeid(long))
						{
							cmd.Param(D.name).setAsLong() = *(long*)void_pointer;
						}
						else if (std::is_same<decltype(item->*(D).pointer), bool&>::value || typeid(item->*(D).pointer) == typeid(bool))
						{
							cmd.Param(D.name).setAsBool() = *(bool*)void_pointer;
						}
						else if (std::is_same<decltype(item->*(D).pointer), double&>::value || typeid(item->*(D).pointer) == typeid(double)
							|| std::is_same<decltype(item->*(D).pointer), float&>::value || typeid(item->*(D).pointer) == typeid(float))
						{
							cmd.Param(D.name).setAsDouble() = *(double*)void_pointer;
						}
						else if (std::is_same<decltype(item->*(D).pointer), std::string&>::value || typeid(item->*(D).pointer) == typeid(std::string)
							|| std::is_same<decltype(item->*(D).pointer), std::wstring&>::value || typeid(item->*(D).pointer) == typeid(std::wstring)
							|| std::is_same<decltype(item->*(D).pointer), std::string_view&>::value || typeid(item->*(D).pointer) == typeid(std::string_view)
							|| std::is_same<decltype(item->*(D).pointer), std::wstring_view&>::value || typeid(item->*(D).pointer) == typeid(std::wstring_view))
						{
							cmd.Param(_TSA(D.name)).setAsString() = (*(std::string*)void_pointer).c_str();
						}
						else if (std::is_same<decltype(item->*(D).pointer), std::tm&>::value || typeid(item->*(D).pointer) == typeid(std::tm))
						{
							cmd.Param(_TSA(D.name)).setAsDateTime() = SADateTime(*(tm*)void_pointer);
						}
					}
				}
			});
			cmd.Param(_TSA("id")).setAsString() = item->GetId().c_str();
			cmd.Execute();
			return cmd.RowsAffected();
		}
		catch (SAException& ex)
		{
			std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
			throw std::exception("Internal error! Database update command failed");
		}
		return 0;
	}

	virtual int Delete(std::string& id) override
	{
		try
		{
			std::string table_name = typeid(T).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string query = "DELETE FROM [dbo].[" + table_name + "] WHERE Id = :id";
			SACommand cmd(con);
			cmd.setCommandText(_TSA(query.c_str()));
			cmd.Param(_TSA("id")).setAsString() = id.c_str();
			cmd.Execute();
			return cmd.RowsAffected();
		}
		catch (SAException& ex)
		{
			std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
			throw std::exception("Internal error! Database delete command failed");
		}
	}

	~BaseCommand() = default;
protected:
	SAConnection* con;
};