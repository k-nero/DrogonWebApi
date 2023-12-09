#include "AuthController.h"
#include <ApplicationUserCommand.h>
#include <ApplicationUser.h>
#include <DbContext.h>
#include "BcryptPW.h"

void AuthController::login( const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback )
{
	DbContext db;
	auto con = db.GetConnection();
	ApplicationUserCommand cmd( con );
	auto& request = req->getJsonObject();
	std::string username = (*request)["username"].asString();
	std::string password = (*request)["password"].asString();
	std::shared_ptr<ApplicationUser>user = cmd.GetApplicationUserByUserName( username );
	if ( user != nullptr )
	{
		if (Bcrypt::VerifyPassword(password, user->GetPasswordHash()))
		{
			Json::Value ret;
			ret = user->ToJson();
			auto resp = HttpResponse::newHttpJsonResponse( ret );
			resp->setStatusCode( k200OK );
			callback( resp );
			return;
		}
	}
	Json::Value ret;
	ret["message"] = "Invalid username or password";
	ret["status"] = k401Unauthorized;
	const auto resp = HttpResponse::newHttpJsonResponse(ret);
	resp->setStatusCode( k401Unauthorized );
	if ( con != nullptr )
	{
		con = nullptr;
	}
	callback( resp );

}

void AuthController::registerUser(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	DbContext db;
	auto con = db.GetConnection();
	ApplicationUserCommand cmd(con);
	auto &reqJson = req->getJsonObject();

	if (!(*reqJson)["username"].isNull())
	{
		auto user = cmd.GetApplicationUserByUserName((*reqJson)["username"].asString());
		if (user != nullptr)
		{
			Json::Value ret;
			ret["status"] = k200OK;
			ret["message"] = "Username already exists";
			ret["id"] = user->GetId();
			const auto resp = HttpResponse::newHttpJsonResponse(ret);
			resp->setStatusCode(k200OK);
			callback(resp);
			return;
		}
	}

	if (reqJson != nullptr)
	{
		UUID uuid;
		std::string guid;
		if (UuidCreate(&uuid) == RPC_S_OK)
		{
			RPC_CSTR szUuid = NULL;
			if (UuidToStringA(&uuid, &szUuid) == RPC_S_OK)
			{
				guid = (char*)szUuid;
				RpcStringFreeA(&szUuid);
			}
		}
		std::string passwordHash = Bcrypt::HashPassword((*reqJson)["password"].asString());
		ApplicationUser user(guid, (*reqJson)["username"].asString(), (*reqJson)["email"].asString(), passwordHash, (*reqJson)["phoneNumber"].asString());
		int rs = cmd.CreateApplicationUser(&user);
		if (rs >= 1)
		{
			Json::Value ret;
			ret["message"] = "User created successfully!";
			ret["status"] = k200OK;
			ret["id"] = guid;
			ret["user"] = user.ToJson();
			const auto resp = HttpResponse::newHttpJsonResponse(ret);
			resp->setStatusCode(k200OK);
			callback(resp);
			return;
		}
	}
	const auto resp = HttpResponse::newHttpResponse();
	resp->setStatusCode(k400BadRequest);
	callback(resp);
}
