#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class AuthController : public HttpController<AuthController>
{
public: 
	static void initPathRouting()
	{
		METHOD_ADD( AuthController::login, "/login", Post ); // path is /AuthController
		return;
	}

	void login ( const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback );
};

