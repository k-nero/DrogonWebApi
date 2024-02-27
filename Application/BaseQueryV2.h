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

			boost::mp11::mp_for_each< boost::describe::describe_members<K, boost::describe::mod_any_access | boost::describe::mod_inherited>>([&](auto D)
			{
				std::string name = D.name;
				query += table_name + "." + name + " as " + name;
				query += ",";
			});


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
};