#pragma once
#include <jwt-cpp/jwt.h> 
#include <drogon/drogon.h>
#include "ConfigProvider.h"
#include "ApplicationUser.h"
#include "ApplicationUserCommand.h"
#include <boost/log/trivial.hpp>

namespace Auth
{
	class Authorization : public drogon::HttpFilter<Authorization>
	{
	public:
		Authorization();
		virtual void doFilter(const drogon::HttpRequestPtr& req, drogon::FilterCallback&& fcb, drogon::FilterChainCallback&& fccb) override;
		virtual ~Authorization();
	};

	class Admin : public drogon::HttpFilter<Admin>
	{
	public:
		Admin();
		virtual void doFilter(const drogon::HttpRequestPtr& req, drogon::FilterCallback&& fcb, drogon::FilterChainCallback&& fccb) override;
		virtual ~Admin();
	};

	class User : public drogon::HttpFilter<User>
	{
	public:
		User();
		virtual void doFilter(const drogon::HttpRequestPtr& req, drogon::FilterCallback&& fcb, drogon::FilterChainCallback&& fccb) override;
		virtual ~User();
	};
}