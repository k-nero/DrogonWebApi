#pragma once
#include "ApplicationApi.h"
#include "IBaseQuery.h"
#include "iostream"
#include "CoreHelper.h"
#include "ApplicationApi.h"
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <boost/log/trivial.hpp>
#include "TypeCheck.h"
#include "TodoItem.h"

template <typename T, class D = boost::describe::describe_members<T, boost::describe::mod_any_access | boost::describe::mod_inherited>>
class APPLICATION_API BaseQuery : public IBaseQuery<T>
{
public:
	BaseQuery() = default;
	explicit BaseQuery(SAConnection* con) { this->con = con; }
	virtual std::shared_ptr<T> GetById(const std::string& id, std::vector<std::string> includes = {}) noexcept(false) override
	{
		try
		{
			if (id.empty())
			{
				throw std::exception("Id is empty");
			}

			std::string table_name = typeid(T).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string query = "SELECT * FROM [dbo].[" + table_name + "] WHERE Id = :id";
			SACommand cmd(con, _TSA(query.c_str()));
			const SAString idStr(id.c_str());
			cmd.Param(_TSA("id")).setAsString() = idStr;
			cmd.Execute();
			auto item = std::make_shared<T>();
			if (cmd.FetchNext())
			{
				boost::mp11::mp_for_each<D>([&](auto D) 
				{
					if(std::find(includes.begin(), includes.end(), D.name) != includes.end())
					{
						using type = std::remove_reference_t<decltype(item.get()->*(D).pointer)>;
						using inner_type = has_value_type_t<type>;
						if (!std::is_void_v<inner_type>)
						{
							using inner_elem_type = has_element_type_t<std::remove_reference_t<inner_type>>;

							if (!std::is_void_v<inner_elem_type>)
							{
								//(type&)(item.get()->*(D).pointer) = (type&)GetAll<inner_elem_type>(table_name + "Id = '" + id + "'");
							}
						}
						
					}
				
				});


				item = GetFromCommand(cmd);
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

	virtual std::vector<std::shared_ptr<T>>GetAll(std::string query = "") noexcept(false) override
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

	virtual std::shared_ptr<T> GetFromCommand(SACommand& cmd, std::vector<std::string> includes = {}) noexcept(false) override
	{
		T* item = new T();
		boost::mp11::mp_for_each<D>([&](auto D)
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
			else if (std::is_same<std::remove_reference_t<decltype(item->*(D).pointer)>, std::tm>::value )
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

	virtual std::shared_ptr<T> GetSingle(const std::string query = "") noexcept(false) override
	{
		try
		{
			std::string table_name = typeid(T).name();
			table_name = table_name.substr(table_name.find_last_of(' ') + 1);
			std::string base_query = "SELECT * FROM [dbo].[" + table_name + "] WHERE " + query;
			SACommand cmd(con, _TSA(base_query.c_str()));
			cmd.Execute();
			if (cmd.FetchNext())
			{
				return GetFromCommand(cmd);
			}
			return nullptr;
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

	virtual PaginationObject<T> GetPagination(int page, int pageSize, std::string query = "") noexcept(false) override
	{
		try
		{
			std::string table_name = typeid(T).name();
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
			std::vector<std::shared_ptr<T>> items;
			while (cmd2.FetchNext())
			{
				items.push_back(GetFromCommand(cmd2));
			}
			return PaginationObject<T>(items, count, page, pageSize);
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

	virtual ~BaseQuery() = default;

protected:
	SAConnection* con = nullptr;
};
