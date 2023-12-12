#include "AuthController.h"


void AuthController::login(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	try
	{
		DbContext db;
		auto con = db.GetConnection();
		ApplicationUserCommand cmd(con);
		auto& request = req->getJsonObject();
		std::string username = (*request)["username"].asString();
		std::string password = (*request)["password"].asString();
		std::shared_ptr<ApplicationUser>user = cmd.GetApplicationUserByUserName(username);
		Json::Value ret;
		if (user != nullptr)
		{
			if (Bcrypt::VerifyPassword(password, user->GetPasswordHash()))
			{
				std::string private_key = ConfigProvider::GetInstance()->GetPrivateRSAKey();
				std::string public_key = ConfigProvider::GetInstance()->GetPublicRSAKey();
				auto token = jwt::create()
					.set_issuer("auth0")
					.set_type("JWT")
					.set_id(CoreHelper::GetGuid())
					.set_payload_claim("sub", jwt::claim(user->GetId()))
					.set_payload_claim("name", jwt::claim(user->GetUserName()))
					.set_payload_claim("role", jwt::claim(std::string{ "Admin" }))
					.set_payload_claim("iat", jwt::claim(std::chrono::system_clock::now()))
					.set_payload_claim("exp", jwt::claim(std::chrono::system_clock::now() + std::chrono::hours{ 24 }))
					.sign(jwt::algorithm::rs512(public_key, private_key, "", ""));
				ret["user"] = ToJson(*user);
				ret["access_token"] = token;
				auto resp = HttpResponse::newHttpJsonResponse(ret);
				resp->setStatusCode(k200OK);
				callback(resp);
				return;
			}
		}
		ret["message"] = "Invalid username or password";
		ret["status"] = k401Unauthorized;
		auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k401Unauthorized);
		callback(resp);
		return;
	}
	catch (...)
	{
		Json::Value ret;
		ret["message"] = "Internal server error";
		ret["status"] = k500InternalServerError;
		auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k500InternalServerError);
		callback(resp);
		return;
	}
}

void AuthController::registerUser(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	try
	{
		DbContext db;
		auto con = db.GetConnection();
		ApplicationUserCommand cmd(con);
		auto& reqJson = req->getJsonObject();
		Json::Value ret;
		if (!(*reqJson)["username"].isNull())
		{
			auto user = cmd.GetApplicationUserByUserName((*reqJson)["username"].asString());
			if (user != nullptr)
			{
				ret["status"] = k200OK;
				ret["message"] = "Username already exists";
				ret["id"] = user->GetId();
				auto resp = HttpResponse::newHttpJsonResponse(ret);
				resp->setStatusCode(k200OK);
				callback(resp);
				return;
			}
			else
			{
				std::string guid = CoreHelper::GetGuid();
				std::string passwordHash = Bcrypt::HashPassword((*reqJson)["password"].asString());
				ApplicationUser user(guid, (*reqJson)["username"].asString(), (*reqJson)["email"].asString(), passwordHash, (*reqJson)["phoneNumber"].asString());
				int rs = cmd.CreateApplicationUser(&user);
				if (rs >= 1)
				{
					ret["message"] = "User created successfully!";
					ret["status"] = k200OK;
					ret["user"] = ToJson(user);
					auto resp = HttpResponse::newHttpJsonResponse(ret);
					resp->setStatusCode(k200OK);
					callback(resp);
					return;
				}
			}
			auto resp = HttpResponse::newHttpResponse();
			resp->setStatusCode(k422UnprocessableEntity);
			callback(resp);
			return;
		}
	}
	catch(...)
	{
		Json::Value ret;
		ret["message"] = "Internal server error";
		ret["status"] = k500InternalServerError;
		auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k500InternalServerError);
		callback(resp);
		return;
	}
}

