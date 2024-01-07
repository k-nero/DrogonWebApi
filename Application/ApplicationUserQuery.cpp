#include "pch.h"
#include "ApplicationUserQuery.h"

ApplicationUserQuery::ApplicationUserQuery()
{
}

std::shared_ptr<ApplicationUser> ApplicationUserQuery::GetByUserName(std::string username)
{
	try
	{
		SACommand cmd(con, _TSA("SELECT * FROM [dbo].[ApplicationUser] WHERE UserName=:username"));
		cmd.Param(_TSA("username")).setAsString() = username.c_str();
		cmd.Execute();
		if (cmd.FetchNext())
		{
			return BaseQuery::GetFromCommand(cmd);
		}
	}
	catch (SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
		throw std::exception("Internal error! Database query failed");
	}
	return nullptr;
}

ApplicationUserQuery::~ApplicationUserQuery()
{

} 