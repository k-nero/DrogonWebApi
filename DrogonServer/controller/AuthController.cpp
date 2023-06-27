#include "AuthController.h"
#include <ApplicationUserCommand.h>
#include <ApplicationUser.h>
#include <DbContext.h>
#include <bcrypt.h>

void AuthController::login( const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback )
{
	DbContext db;
	auto con = db.GetConnection();
	ApplicationUserCommand cmd( con );
	const auto& username = req->getParameter( "username" );
	ApplicationUser* user = cmd.GetApplicationUserByUserName( username );
	if ( user != nullptr )
	{
		const auto& password = req->getParameter( "password" );
		if ( user->GetPasswordHash() == password)
		{
			const auto resp = HttpResponse::newHttpResponse();
			resp->setStatusCode( k200OK );
			resp->addHeader( "Content-Type", "application/json; charset=utf-8" );
			resp->addHeader( "Set-Cookie", "username=" + username + "; Path=/; HttpOnly" );
			callback( resp );
			return;
		}
	}
	Json::Value ret;
	if ( user != nullptr )
	{
		ret["Id"] = user->GetId();
		ret["Username"] = user->GetUserName();
		ret["PasswordHash"] = user->GetPasswordHash();
		ret["SecurityStamp"] = user->GetSecurityStamp();
		ret["Email"] = user->GetEmail();
		ret["EmailConfirmed"] = user->GetEmailConfirmed();
		ret["PhoneNumber"] = user->GetPhoneNumber();
		ret["PhoneNumberConfirmed"] = user->GetPhoneNumberConfirmed();
		ret["TwoFactorEnabled"] = user->GetTwoFactorEnabled();
		ret["AccessFailedCount"] = user->GetAccessFailedCount();
	}
	const auto resp = HttpResponse::newHttpJsonResponse( ret );
	resp->setStatusCode( k200OK );
	if ( user != nullptr )
	{
		delete user;
		user = nullptr;
	}
	if ( con != nullptr )
	{
		con = nullptr;
	}
	callback( resp );

}
