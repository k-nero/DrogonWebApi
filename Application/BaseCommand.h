#pragma once
#include "ApplicationApi.h"
#include "IBaseCommand.h"
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <iostream>
#include <boost/log/trivial.hpp>


template <typename T, class D = boost::describe::describe_members<T, boost::describe::mod_any_access | boost::describe::mod_inherited>>
class APPLICATION_API BaseCommand : public IBaseCommand<T>
{
public:
	BaseCommand() = default;
	explicit BaseCommand(SAConnection* con) { this->con = con; }
	virtual int Create(T* item) noexcept(false) override
	{
		try
		{
			if (item == nullptr)
			{
				throw std::exception("Internal error! Item is null");
			}
			if (con == nullptr || !con->isConnected())
			{
				throw std::exception("Internal error! Database connection is null or failed");
			}
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
						if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, int>::value )
						{
							cmd.Param(D.name).setAsLong() = *(int*)void_pointer;
						}
						else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, long>::value )
						{
							cmd.Param(D.name).setAsLong() = *(long*)void_pointer;
						}
						else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, bool>::value )
						{
							cmd.Param(D.name).setAsBool() = *(bool*)void_pointer;
						}
						else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, double>::value
							|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, float>::value )
						{
							cmd.Param(D.name).setAsDouble() = *(double*)void_pointer;
						}
						else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::string>::value 
							|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::wstring>::value 
							|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::string_view>::value 
							|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::wstring_view>::value )
						{
							cmd.Param(_TSA(D.name)).setAsString() = (*(std::string*)void_pointer).c_str();
						}
						else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::tm>::value )
						{
							cmd.Param(_TSA(D.name)).setAsDateTime() = SADateTime(*(tm*)void_pointer);
						}
					}
				}
			});
			cmd.Execute();
			return (int)cmd.RowsAffected();
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

	virtual int Update(T* item) noexcept(false) override
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
						if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, int>::value )
						{
							cmd.Param(D.name).setAsLong() = *(int*)void_pointer;
						}
						else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, long>::value )
						{
							cmd.Param(D.name).setAsLong() = *(long*)void_pointer;
						}
						else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, bool>::value )
						{
							cmd.Param(D.name).setAsBool() = *(bool*)void_pointer;
						}
						else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, double>::value 
							|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, float>::value)
						{
							cmd.Param(D.name).setAsDouble() = *(double*)void_pointer;
						}
						else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::string>::value 
							|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::wstring>::value 
							|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::string_view>::value 
							|| std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::wstring_view>::value )
						{
							cmd.Param(_TSA(D.name)).setAsString() = (*(std::string*)void_pointer).c_str();
						}
						else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::tm>::value )
						{
							cmd.Param(_TSA(D.name)).setAsDateTime() = SADateTime(*(tm*)void_pointer);
						}
					}
				}
			});
			cmd.Param(_TSA("id")).setAsString() = item->GetId().c_str();
			cmd.Execute();
			return (int)cmd.RowsAffected();
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

	virtual int Delete(std::string& id) noexcept(false) override
	{
		try
		{
			if (id.empty())
			{
				throw std::exception("Internal error! Id is empty");
			}
			std::string table_name = typeid(T).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string query = "DELETE FROM [dbo].[" + table_name + "] WHERE Id = :id";
			SACommand cmd(con);
			cmd.setCommandText(_TSA(query.c_str()));
			cmd.Param(_TSA("id")).setAsString() = id.c_str();
			cmd.Execute();
			return (int)cmd.RowsAffected();
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

	~BaseCommand() = default;
protected:
	SAConnection* con = nullptr;
};