#pragma once

#include <jwt-cpp/jwt.h>
#include <drogon/HttpController.h>
#include "CoreHelper.h"
#include "ApplicationUserCommand.h"
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
		registerMethod(&AuthController::login, "/api/auth/login", { Post }, false, "AuthController::login");
		registerMethod(&AuthController::registerUser, "/api/auth/register", { Post }, false, "AuthController::registerUser");
	}
	void login ( const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback );
	void registerUser ( const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback );
};

