#include "pch.h"
#include "ApplicationUserCommand.h"

#include <iostream>
#include <ostream>
#include <SQLAPI.h>


ApplicationUserCommand::ApplicationUserCommand() = default;

ApplicationUser* ApplicationUserCommand::GetApplicationUserById( const std::string& id ) const
{
	try
	{
		SACommand cmd( con, _TSA( "SELECT * FROM [dbo].[AspNetUsers] WHERE Id=:id" ) );
		const SAString idStr( id.c_str() );
		cmd.Param( _TSA( "id" ) ).setAsString() = idStr;
		cmd.Execute();
		if ( cmd.FetchNext() )
		{
			return new ApplicationUser( cmd.Field( _TSA( "Id" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "UserName" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "NormalizedUserName" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "Email" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "EmailConfirmed" ) ).asBool(),
				cmd.Field( _TSA( "NormalizedEmail" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "PasswordHash" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "SecurityStamp" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "ConcurrencyStamp" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "PhoneNumber" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "PhoneNumberConfirmed" ) ).asBool(),
				cmd.Field( _TSA( "TwoFactorEnabled" ) ).asBool(),
				cmd.Field( _TSA( "LockoutEnd" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "LockoutEnabled" ) ).asBool(),
				cmd.Field( _TSA( "AccessFailedCount" ) ).asUInt64() );
		}
	}
	catch ( SAException& ex )
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
	}
	return nullptr;
}

ApplicationUser* ApplicationUserCommand::GetApplicationUserByUserName( std::string userName )
{
	try
	{
		SACommand cmd( con, _TSA( "SELECT * FROM [dbo].[AspNetUsers] WHERE UserName=:username" ) );
		const SAString username( userName.c_str() );
		cmd.Param( _TSA( "username" ) ).setAsString() = username;
		cmd.Execute();
		if ( cmd.FetchNext() )
		{
			return new ApplicationUser( cmd.Field( _TSA( "Id" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "UserName" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "NormalizedUserName" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "Email" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "EmailConfirmed" ) ).asBool(),
				cmd.Field( _TSA( "NormalizedEmail" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "PasswordHash" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "SecurityStamp" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "ConcurrencyStamp" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "PhoneNumber" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "PhoneNumberConfirmed" ) ).asBool(),
				cmd.Field( _TSA( "TwoFactorEnabled" ) ).asBool(),
				cmd.Field( _TSA( "LockoutEnd" ) ).asString().GetMultiByteChars(),
				cmd.Field( _TSA( "LockoutEnabled" ) ).asBool(),
				cmd.Field( _TSA( "AccessFailedCount" ) ).asUInt64() );
		}
	}
	catch ( SAException& ex )
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
	}
	return nullptr;
}

std::vector<ApplicationUser*> ApplicationUserCommand::GetAllApplicationUsers()
{
	std::vector<ApplicationUser*> applicationUsers;
	try
	{
		SACommand cmd(con, _TSA("SELECT * FROM [dbo].[AspNetUsers]"));
		cmd.Execute();
		while (cmd.FetchNext())
		{
			auto user = new ApplicationUser(cmd.Field(_TSA("Id")).asString().GetMultiByteChars(),
				cmd.Field(_TSA("UserName")).asString().GetMultiByteChars(),
				cmd.Field(_TSA("NormalizedUserName")).asString().GetMultiByteChars(),
				cmd.Field(_TSA("Email")).asString().GetMultiByteChars(),
				cmd.Field(_TSA("EmailConfirmed")).asBool(),
				cmd.Field(_TSA("NormalizedEmail")).asString().GetMultiByteChars(),
				cmd.Field(_TSA("PasswordHash")).asString().GetMultiByteChars(),
				cmd.Field(_TSA("SecurityStamp")).asString().GetMultiByteChars(),
				cmd.Field(_TSA("ConcurrencyStamp")).asString().GetMultiByteChars(),
				cmd.Field(_TSA("PhoneNumber")).asString().GetMultiByteChars(),
				cmd.Field(_TSA("PhoneNumberConfirmed")).asBool(),
				cmd.Field(_TSA("TwoFactorEnabled")).asBool(),
				cmd.Field(_TSA("LockoutEnd")).asString().GetMultiByteChars(),
				cmd.Field(_TSA("LockoutEnabled")).asBool(),
				cmd.Field(_TSA("AccessFailedCount")).asUInt64());
			applicationUsers.push_back(user);
		}
	}
	catch (SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
	}
	return applicationUsers;
}

std::string ApplicationUserCommand::CreateApplicationUser( ApplicationUser* applicationUser )
{
	try
	{
		return std::string();
	}
	catch( SAException& ex)
	{
		std::cout << ex.ErrText().GetMultiByteChars() << std::endl;
	}
	return std::string();
}

ApplicationUserCommand::~ApplicationUserCommand()
{
	if(this->con != nullptr )
	{
		con = nullptr;
	}
}
