#pragma once
#include <jwt-cpp/jwt.h> 
#include <drogon/drogon.h>
#include "ConfigProvider.h"
#include "ApplicationUser.h"
#include "ApplicationUserCommand.h"



class Authorization : public drogon::HttpFilter<Authorization>
{
public:
	Authorization();
	virtual void doFilter(const drogon::HttpRequestPtr& req, drogon::FilterCallback&& fcb, drogon::FilterChainCallback&& fccb) override;
	virtual ~Authorization();
};

