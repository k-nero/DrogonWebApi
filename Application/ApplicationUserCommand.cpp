#include "pch.h"
#include "ApplicationUserCommand.h"
#include <SQLAPI.h>


ApplicationUserCommand::ApplicationUserCommand() = default;

ApplicationUser* ApplicationUserCommand::GetApplicationUserById( const std::string& id ) const
{
	SACommand cmd( con, _TSA( "SELECT * FROM [dbo].[AspNetUsers] WHERE Id=:id" ) );
	const SAString idStr = id.c_str();
	cmd.Param( _TSA( "id" ) ).setAsString() = idStr;
	cmd.Execute();
	while ( cmd.FetchNext() )
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
			cmd.Field( _TSA( "AccessFailedCount" ) ).asInt64() );
	}
	return nullptr;
}

ApplicationUser* ApplicationUserCommand::GetApplicationUserByUserName( std::string userName )
{
	return nullptr;
}

std::vector<ApplicationUser*> ApplicationUserCommand::GetAllApplicationUsers()
{
	return {};
}

ApplicationUserCommand::~ApplicationUserCommand() = default;
