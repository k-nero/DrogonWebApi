#pragma once

#include <drogon/HttpController.h>
#include <ApplicationUserCommand.h>
#include "TestApplicationUser.h"
#include <ApplicationUser.h>
#include <DbContext.h>
#include <memory.h>
#include "JsonHelper.h"

using namespace drogon;


class IndexController : public HttpController<IndexController>
{
public:
	static void initPathRouting()
	{
		registerMethod(&IndexController::Get, "/api/users/{1}", { drogon::HttpMethod::Get }, false, "IndexController::Get"); // path is /IndexController
		registerMethod(&IndexController::GetAll, "/api/users", { drogon::HttpMethod::Get , "Auth::Authorization", "Auth::Admin"}, false, "IndexController::GetAll");
	}
	// your declaration of processing function maybe like this:
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Get( const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback, std::string p1 );
};
