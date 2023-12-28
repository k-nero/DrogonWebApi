#pragma once
#include <SQLAPI.h>
#include <vector>
#include "CoreHelper.h"
#include "ApplicationApi.h"

template<typename T>
__interface APPLICATION_API BaseCommand
{
	virtual BaseCommand() = 0;
	virtual std::shared_ptr<T> GetById(const std::string& id)
	{
		try
		{
			std::string query = "SELECT * FROM [dbo].[" + typeid(T).name() + "] WHERE Id = :id";
			SACommand cmd(con, _TSA("SELECT * FROM [dbo].[Users] WHERE Id=:id"));
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
	virtual std::vector<std::shared_ptr<T>>GetAll(std::string query = "")
	{
		std::vector<std::shared_ptr<T>> items;
		try
		{
			std::string base_query = "SELECT * FROM [dbo].[" + typeid(T).name() + "]";
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
	virtual int Create(T* item);
	virtual int Update(T* item);
	virtual std::shared_ptr<T> GetFromCommand(SACommand& cmd)
	{

	}
	virtual ~BaseCommand();
};