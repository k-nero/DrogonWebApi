#include "pch.h"
#include "ApplicationUserCommand.h"
#include <iostream>
#include <ostream>
#include <SQLAPI.h>

ApplicationUserCommand::ApplicationUserCommand() = default;

std::shared_ptr<ApplicationUser> ApplicationUserCommand::GetApplicationUserFromCommand(SACommand& cmd)
{
	return std::shared_ptr<ApplicationUser>(new ApplicationUser(
		cmd.Field(_TSA("Id")).asString().GetMultiByteChars(),
		cmd.Field(_TSA("UserName")).asString().GetMultiByteChars(),
		cmd.Field(_TSA("Email")).asString().GetMultiByteChars(),
		cmd.Field(_TSA("PasswordHash")).asString().GetMultiByteChars(),
		cmd.Field(_TSA("PhoneNumber")).asString().GetMultiByteChars()));
}

std::shared_ptr<ApplicationUser> ApplicationUserCommand::GetApplicationUserById( const std::string& id ) 
{
	try
	{
		SACommand cmd( con, _TSA( "SELECT * FROM [dbo].[Users] WHERE Id=:id" ) );
		const SAString idStr( id.c_str() );
		cmd.Param( _TSA( "id" ) ).setAsString() = idStr;
		cmd.Execute();
		if ( cmd.FetchNext() )
		{
			return GetApplicationUserFromCommand(cmd);
		}
	}
	catch ( SAException& ex )
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
	}
	return nullptr;
}

std::shared_ptr<ApplicationUser> ApplicationUserCommand::GetApplicationUserByUserName( std::string userName )
{
	try
	{
		SACommand cmd( con, _TSA( "SELECT * FROM [dbo].[Users] WHERE UserName=:username" ) );
		const SAString username( userName.c_str() );
		cmd.Param( _TSA( "username" ) ).setAsString() = username;
		cmd.Execute();
		if ( cmd.FetchNext() )
		{
			return GetApplicationUserFromCommand(cmd);
		}
	}
	catch ( SAException& ex )
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
	}
	return nullptr;
}

std::vector<std::shared_ptr<ApplicationUser>> ApplicationUserCommand::GetAllApplicationUsers()
{
	std::vector<std::shared_ptr<ApplicationUser>> applicationUsers;
	try
	{
		SACommand cmd(con, _TSA("SELECT * FROM [dbo].[Users]"));
		cmd.Execute();
		while (cmd.FetchNext())
		{
			applicationUsers.push_back(GetApplicationUserFromCommand(cmd));
		}
		return applicationUsers;
	}
	catch (SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
	}
	return applicationUsers;
}


int ApplicationUserCommand::CreateApplicationUser(ApplicationUser* applicationUser)
{
	try
	{
		SACommand cmd(con, _TSA("INSERT INTO [dbo].[Users] (Id, UserName, PasswordHash, Email, PhoneNumber, CreatedDate) VALUES (:Id, :Username, :PasswordHash, :Email, :PhoneNumber, :CreatedDate)"));
		cmd.Param(_TSA("id")).setAsString() = _TSA(applicationUser->GetId().c_str());
		cmd.Param(_TSA("UserName")).setAsString() = _TSA(applicationUser->GetUserName().c_str());
		cmd.Param(_TSA("PasswordHash")).setAsString() = _TSA(applicationUser->GetPasswordHash().c_str());
		cmd.Param(_TSA("Email")).setAsString() = _TSA(applicationUser->GetEmail().c_str());
		cmd.Param(_TSA("PhoneNumber")).setAsString() = _TSA(applicationUser->GetPhoneNumber().c_str());
		cmd.Param(_TSA("CreatedDate")).setAsDateTime() = SADateTime(CoreHelper::GetSystemTime());
		cmd.Execute();
		return cmd.RowsAffected();
	}
	catch (SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
	}
	return 0;
}

int ApplicationUserCommand::UpdateApplicationUser(ApplicationUser* applicationUser)
{
	try
	{
		SACommand cmd(con, _TSA("UPDATE [dbo].[Users] SET UserName=:UserName, PasswordHash=:PasswordHash, Email=:Email, PhoneNumber=:PhoneNumber, ModifiedDate=:ModifiedDate WHERE Id=:Id"));
		cmd.Param(_TSA("Id")).setAsString() = _TSA(applicationUser->GetId().c_str());
		cmd.Param(_TSA("UserName")).setAsString() = _TSA(applicationUser->GetUserName().c_str());
		cmd.Param(_TSA("PasswordHash")).setAsString() = _TSA(applicationUser->GetPasswordHash().c_str());
		cmd.Param(_TSA("Email")).setAsString() = _TSA(applicationUser->GetEmail().c_str());
		cmd.Param(_TSA("PhoneNumber")).setAsString() = _TSA(applicationUser->GetPhoneNumber().c_str());
		cmd.Param(_TSA("ModifiedDate")).setAsDateTime() = SADateTime(CoreHelper::GetSystemTime());
		cmd.Execute();
		return cmd.RowsAffected();
	}
	catch (SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
	}
	return 0;
}

ApplicationUserCommand::~ApplicationUserCommand()
{
	if(this->con != nullptr )
	{
		con = nullptr;
	}
}

