#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class AuthController : public HttpController<AuthController>
{
public: 
	static void initPathRouting()
	{
		METHOD_ADD( AuthController::login, "/login", Post ); // path is /AuthController
		METHOD_ADD(AuthController::registerUser, "/register", Post);
		return;
	}
	void login ( const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback );
	void registerUser ( const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback );
};

