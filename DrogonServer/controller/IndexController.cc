#include "IndexController.h"
#include <ApplicationUserCommand.h>
#include <ApplicationUser.h>
#include <DbContext.h>

void IndexController::get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string p1)
{
	DbContext db; 
	auto con = db.GetConnection();
	ApplicationUserCommand cmd(con);
	ApplicationUser * user = cmd.GetApplicationUserById( p1 );
	Json::Value ret;
	if(user != nullptr )
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
	const auto resp = HttpResponse::newHttpJsonResponse(ret);
	resp->setStatusCode( k200OK );
	if(user != nullptr )
	{
		delete user;
		user = nullptr;
	}
	if(con != nullptr )
	{
		con = nullptr;
	}
	callback(resp);
}
