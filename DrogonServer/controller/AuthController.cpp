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
	std::shared_ptr<ApplicationUser>user = cmd.GetApplicationUserByUserName( username );
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
		ret = user->ToJson();
	}
	const auto resp = HttpResponse::newHttpJsonResponse( ret );
	resp->setStatusCode( k200OK );
	if ( con != nullptr )
	{
		con = nullptr;
	}
	callback( resp );

}

void AuthController::registerUser(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	auto &reqJson = req->getJsonObject();
	const auto resp = HttpResponse::newHttpResponse();
	resp->setStatusCode(k400BadRequest);
	if (reqJson != nullptr)
	{
		std::cout << (*reqJson)["username"].asString() << std::endl;
		UUID uuid;
		::UuidCreate(&uuid);
		std::string guid;
		RPC_CSTR szUuid = NULL;
		if (::UuidToStringA(&uuid, &szUuid) == RPC_S_OK)
		{
			guid = (char*)szUuid;
			::RpcStringFreeA(&szUuid);
		}
		ApplicationUser user(guid, (*reqJson)["username"].asString(), (*reqJson)["email"].asString(), (*reqJson)["passwordHash"].asString(), (*reqJson)["phoneNumber"].asString());
		DbContext db;
		auto con = db.GetConnection();
		ApplicationUserCommand cmd(con);
		int rs = cmd.CreateApplicationUser(&user);
		if (rs >= 1)
		{
			resp->setStatusCode(k200OK);
			resp->addHeader("Content-Type", "application/json; charset=utf-8");
			Json::Value ret;
			ret["affectedRow"] = rs;
		}
	}
	else
	{
		resp->setStatusCode(k400BadRequest);
	}
	callback(resp);
}
