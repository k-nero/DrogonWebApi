#include "Authorization.h"

Authorization::Authorization()
{
}

void Authorization::doFilter(const drogon::HttpRequestPtr& req, drogon::FilterCallback&& fcb, drogon::FilterChainCallback&& fccb)
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
	auto token = rawToken.substr(rawToken.find_first_of(" ") + 1);
	auto privateKey = ConfigProvider::GetInstance()->GetPrivateRSAKey();
	auto publicKey = ConfigProvider::GetInstance()->GetPublicRSAKey();
	auto verifier = jwt::verify().allow_algorithm(jwt::algorithm::rs512(publicKey, privateKey, "", "")).with_issuer("auth0");
	try
	{
		auto decoded = jwt::decode(token);
		verifier.verify(decoded);
		auto payload = decoded.get_payload_claims();
		auto userId = payload.find("sub");
		fccb();
	}
	catch (const std::exception& e)
	{
		Json::Value json;
		json["debug_message"] = e.what();
		json["message"] = "An error occured while decoding token";
		auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
		resp->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
		fcb(resp);
		return;
	}
	catch (const jwt::signature_verification_exception& e)
	{
		Json::Value json;
		json["debug_message"] = e.what();
		json["message"] = "Invalid token";
		auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
		resp->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
		fcb(resp);
		return;
	}
}

Authorization::~Authorization()
{
}
