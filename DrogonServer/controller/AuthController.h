#pragma once

#include <jwt-cpp/jwt.h>
#include <drogon/HttpController.h>
#include "CoreHelper.h"
#include "ApplicationUserServices.h"
#include "ApplicationUser.h"
#include "DbContext.h"
#include "BcryptPW.h"
#include "JsonHelper.h"

using namespace drogon;

class AuthController : public HttpController<AuthController>
{
public: 
	static void initPathRouting()
	{
		registerMethod(&AuthController::Login, "/api/auth/login", { Post }, false, "AuthController::Login");
		registerMethod(&AuthController::Register, "/api/auth/register", { Post }, false, "AuthController::RegisterUser");
	}
	void Login ( const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback );
	void Register ( const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback );
};

