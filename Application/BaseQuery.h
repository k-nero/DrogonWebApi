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

template <typename T,
	class D = boost::describe::describe_members<T, boost::describe::mod_any_access | boost::describe::mod_inherited>>
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

	virtual int Create(T* item) override
	{
		try
		{
			std::string table_name = typeid(T).name();
			std::string query = "INSERT INTO [dbo].[" + table_name + "] (";
			std::string values = " VALUES (";
			SACommand cmd(con);
			cmd.setCommandText(_TSA(query.c_str()));
			boost::mp11::mp_for_each<D>([&](auto D)
			{
				std::string field = D.name;
				if (D.name != "ModifiedDate")
				{
					if (D.name == "CreatedDate")
					{
						query += field + "), ";
						values += "NOW(), ";
					}
					else
					{
						query += field + ", ";
						values += ":" + field + ", ";
						auto value = (item->*(D).pointer);
						auto void_value = (void*)&value;
						if (std::is_same<decltype(item->*(D).pointer), int&>::value)
						{
							cmd.Param(D.name).setAsLong() = *(int*)void_value;
						}
						else if (std::is_same<decltype(item->*(D).pointer), long&>::value)
						{
							cmd.Param(D.name).setAsLong() = *(long*)void_value;
						}
						else if (std::is_same<decltype(item->*(D).pointer), bool&>::value)
						{
							cmd.Param(D.name).setAsBool() = *(bool*)void_value;
						}
						else if (std::is_same<decltype(item->*(D).pointer), double&>::value || std::is_same<decltype(item->*(D).pointer), float&>::value)
						{
							cmd.Param(D.name).setAsDouble() = *(double*)void_value;
						}
						else if (std::is_same<decltype(item->*(D).pointer), std::string&>::value
							|| std::is_same<decltype(item->*(D).pointer), std::wstring&>::value
							|| std::is_same<decltype(item->*(D).pointer), std::string_view&>::value
							|| std::is_same<decltype(item->*(D).pointer), std::wstring_view&>::value)
						{
							cmd.Param(_TSA(D.name)).setAsString() = (item->*(D).pointer).c_str();
						}
						else if (std::is_same<decltype(item->*(D).pointer), std::tm>::value)
						{
							//cmd.Param(_TSA(D.name)).setAsDateTime() = item->*(D).pointer;
						}
					}
				}
			});
			query = query.substr(0, query.length() - 2) + ")";
			values = values.substr(0, values.length() - 2) + ")";
			cmd.setCommandText(_TSA((query + values).c_str()));
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

	//int Update(T* item) override
	//{
	//	try
	//	{
	//		std::string table_name = typeid(T).name();
	//		std::string query = "UPDATE [dbo].[" + table_name + "] SET ";
	//		SACommand cmd(con);
	//		cmd.setCommandText(_TSA(query.c_str()));
	//		boost::mp11::mp_for_each<D>([&](auto D)
	//		{
	//			if (D.name != "Id" && D.name != "CreatedDate")
	//			{
	//				if (D.name == "ModifiedDate")
	//				{
	//					query += D.name + " = NOW(), ";
	//				}
	//				else
	//				{
	//					query += D.name + " = :" + D.name + ", ";
	//					if (std::is_same<decltype(item->*(D).pointer), int>::value)
	//					{
	//						cmd.Param(_TSA(D.name)).setAsLong() = item->*(D).pointer;
	//					}
	//					else if (std::is_same<decltype(item->*(D).pointer), long>::value)
	//					{
	//						cmd.Param(_TSA(D.name)).setAsInt64() = item->*(D).pointer;
	//					}
	//					else if (std::is_same<decltype(item->*(D).pointer), bool>::value)
	//					{
	//						cmd.Param(_TSA(D.name)).setAsBool() = item->*(D).pointer;
	//					}
	//					else if (std::is_same<decltype(item->*(D).pointer), double>::value || std::is_same<decltype(item->*(D).pointer), float>::value)
	//					{
	//						cmd.Param(_TSA(D.name)).setAsDouble() = item->*(D).pointer;
	//					}
	//					else if (std::is_same<decltype(item->*(D).pointer), std::string>::value
	//						|| std::is_same<decltype(item->*(D).pointer), std::wstring>::value
	//						|| std::is_same<decltype(item->*(D).pointer), std::string_view>::value
	//						|| std::is_same<decltype(item->*(D).pointer), std::wstring_view>::value
	//						|| std::is_same<decltype(item->*(D).pointer), const char*>::value
	//						|| std::is_same<decltype(item->*(D).pointer), const wchar_t*>::value
	//						|| std::is_same<decltype(item->*(D).pointer), char*>::value
	//						|| std::is_same<decltype(item->*(D).pointer), wchar_t*>::value
	//						|| std::is_same<decltype(item->*(D).pointer), char[]>::value
	//						|| std::is_same<decltype(item->*(D).pointer), wchar_t[]>::value)
	//					{
	//						cmd.Param(_TSA(D.name)).setAsString() = item->*(D).pointer;
	//					}
	//					else if (std::is_same<decltype(item->*(D).pointer), std::tm>::value)
	//					{
	//						//cmd.Param(_TSA(D.name)).setAsDateTime() = item->*(D).pointer;
	//					}
	//				}
	//			}
	//		});
	//		query = query.substr(0, query.length() - 2) + " WHERE Id = :id";
	//		cmd.setCommandText(_TSA(query.c_str()));
	//		cmd.Param(_TSA("id")).setAsString() = item->GetId().c_str();
	//		cmd.Execute();
	//		return cmd.RowsAffected();
	//	}
	//	catch (SAException& ex)
	//	{
	//		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
	//		throw std::exception("Internal error! Database update command failed");
	//	}
	//	return 0;
	//}

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
			else if (std::is_same<decltype(item->*(D).pointer), int&>::value)
			{
				item->*(D).pointer = cmd.Field(D.name).asLong();
			}
			else if (std::is_same<decltype(item->*(D).pointer), bool&>::value)
			{
				item->*(D).pointer = cmd.Field(D.name).asBool();
			}
			else if (std::is_same<decltype(item->*(D).pointer), double&>::value || std::is_same<decltype(item->*(D).pointer), float&>::value)
			{
				item->*(D).pointer = cmd.Field(D.name).asDouble();
			}
			else if (std::is_same<decltype(item->*(D).pointer), std::string&>::value
				|| std::is_same<decltype(item->*(D).pointer), std::wstring&>::value
				|| std::is_same<decltype(item->*(D).pointer), std::string_view&>::value
				|| std::is_same<decltype(item->*(D).pointer), std::wstring_view&>::value)
			{
				item->*(D).pointer = cmd.Field(D.name).asString().GetMultiByteChars();
			}

			//else if (std::is_same<decltype(item->*(D).pointer), std::tm>::value)
				//item->*(D).pointer = cmd.Field(D.name).asDateTime().GetAsDBTIMESTAMP();
		});
		return std::shared_ptr<T>(item);
	}
	virtual ~BaseQuery() = default;

protected:
	SAConnection* con;
};