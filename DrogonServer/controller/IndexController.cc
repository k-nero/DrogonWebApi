#include "IndexController.h"
#include <ApplicationUserCommand.h>
#include <ApplicationUser.h>
#include <DbContext.h>
// Add definition of your processing function here
void IndexController::get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1)
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
		ret["GetPasswordHash"] = user->GetPasswordHash();
		ret["GetSecurityStamp"] = user->GetSecurityStamp();
		ret["GetEmail"] = user->GetEmail();
		ret["GetEmailConfirmed"] = user->GetEmailConfirmed();
		ret["GetPhoneNumber"] = user->GetPhoneNumber();
		ret["GetPhoneNumberConfirmed"] = user->GetPhoneNumberConfirmed();
		ret["GetTwoFactorEnabled"] = user->GetTwoFactorEnabled();
	}
	const auto resp = HttpResponse::newHttpJsonResponse(ret);
	delete user;
	user = nullptr;
	callback(resp);
}
