#pragma once

#include <jwt-cpp/jwt.h>
#include <drogon/HttpController.h>
#include "CoreHelper.h"
#include "ApplicationUserServices.h"
#include "ApplicationUser.h"
#include "DbContext.h"
#include "BcryptPW.h"
#include "JsonHelper.h"
#include <boost/log/trivial.hpp>

using namespace drogon;

class AuthController : public HttpController<AuthController>
{
public: 
	static void initPathRouting()
	{
		// Deprecated
		registerMethod(&AuthController::Login, "/api/auth/login", { Post }, false, "AuthController::Login");
		registerMethod(&AuthController::Register, "/api/auth/register", { Post }, false, "AuthController::Register");

		registerMethod(&AuthController::FirebaseAuth, "/api/auth/firebase-auth", { Post }, false, "AuthController::FirebaseAuth");
		registerMethod(&AuthController::RefreshToken, "/api/auth/refresh-token", { Post }, false, "AuthController::RefreshToken");
		registerMethod(&AuthController::SignOut, "/api/auth/sign-out", { Post }, false, "AuthController::SignOut");
		registerMethod(&AuthController::SignOutAll, "/api/auth/sign-out-all", { Post }, false, "AuthController::SignOutAll");
	}

	// Deprecated
	void Login ( const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback );
	void Register ( const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback );

	void FirebaseAuth(const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback);
	void RefreshToken(const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback);
	void SignOut(const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback);
	void SignOutAll(const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback);
};

