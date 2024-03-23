#pragma once

#include <drogon/HttpController.h>
#include "ApplicationUserServices.h"
#include <ApplicationUser.h>
#include "ObjToJson.h"

using namespace drogon;

class UserController : public HttpController<UserController>
{
public:
	static void initPathRouting()
	{
		registerMethod(&UserController::Get, "/api/users/{1}", { drogon::HttpMethod::Get }, false, "IndexController::Get"); // path is /IndexController
		registerMethod(&UserController::GetAll, "/api/users", { drogon::HttpMethod::Get , "Auth::Authorization", "Auth::Admin"}, false, "IndexController::GetAll");
	}
	// your declaration of processing function maybe like this:
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Get( const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback, std::string p1 );
};
