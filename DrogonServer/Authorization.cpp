#include "Authorization.h"

Auth::Authorization::Authorization() = default;

void Auth::Authorization::doFilter(const drogon::HttpRequestPtr& req, drogon::FilterCallback&& fcb, drogon::FilterChainCallback&& fccb)
{
	auto& rawToken = req->getHeader("Authorization");
	if (rawToken.empty())
	{
		Json::Value json;
		json["debug_message"] = "No token provided";
		auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
		resp->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
		fcb(resp);
		return;
	}
	if(!rawToken.starts_with("Bearer"))
	{
		Json::Value json;
		json["debug_message"] = "Invalid token format";
		auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
		resp->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
		fcb(resp);
		return;
	}
	auto token = rawToken.substr(rawToken.find_first_of(" ") + 1);
	auto publicKey = ConfigProvider::GetInstance()->GetPublicRSAKey();
	try
	{
		auto decoded = jwt::decode(token);
		jwt::verify().allow_algorithm(jwt::algorithm::rs512(publicKey)).with_issuer("auth0").verify(decoded);
		auto role = decoded.get_payload_claim("role");
		auto userId = decoded.get_payload_claim("sub");
		if (!role.as_string().empty())
		{
			req->addHeader("role", role.as_string());
		}
		if (!userId.as_string().empty())
		{
			req->addHeader("user_id", userId.as_string());
		}
		fccb();
	}
	catch (const std::exception& e)
	{
		Json::Value json;
		json["debug_message"] = e.what();
		auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
		resp->addHeader("Access-Control-Allow-Origin", "*");
		resp->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, PATCH");
		resp->addHeader("Access-Control-Allow-Headers", "*");
		resp->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
		fcb(resp);
		return;
	}
	catch (...)
	{
		BOOST_LOG_TRIVIAL(error) << "Unknow exception";
		Json::Value ret;
		ret["message"] = "Internal server error";
		ret["status"] = drogon::HttpStatusCode::k500InternalServerError;
		auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(drogon::HttpStatusCode::k500InternalServerError);
		fcb(resp);
		return;
	}
}

Auth::Authorization::~Authorization() = default;

Auth::Admin::Admin() = default;

void Auth::Admin::doFilter(const drogon::HttpRequestPtr& req, drogon::FilterCallback&& fcb, drogon::FilterChainCallback&& fccb)
{
	auto& role = req->getHeader("role");
	if (role != "Admin")
	{
		Json::Value json;
		json["debug_message"] = "You are not authorized to access this resource";
		json["status"] = drogon::HttpStatusCode::k403Forbidden;
		auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
		resp->setStatusCode(drogon::HttpStatusCode::k403Forbidden);
		fcb(resp);
		return;
	}
	fccb();
}

Auth::Admin::~Admin() = default;

Auth::User::User() = default;

void Auth::User::doFilter(const drogon::HttpRequestPtr& req, drogon::FilterCallback&& fcb, drogon::FilterChainCallback&& fccb)
{
	auto& role = req->getHeader("role");
	if (role != "User")
	{
		Json::Value json;
		json["debug_message"] = "You are not authorized to access this resource";
		json["status"] = drogon::HttpStatusCode::k403Forbidden;
		auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
		resp->setStatusCode(drogon::HttpStatusCode::k403Forbidden);
		fcb(resp);
		return;
	}
	fccb();
}

Auth::User::~User() = default;
