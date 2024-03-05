#pragma once

#include <drogon/HttpController.h>
#include <memory.h>
#include "JsonHelper.h"
#include <boost/log/trivial.hpp>
#include "CoreHelper.h"

using namespace drogon;

class SwaggerSpec : public HttpController<SwaggerSpec>
{
public:
	static void initPathRouting()
	{
		registerMethod(&SwaggerSpec::Spec, "/swagger/spec", { drogon::HttpMethod::Get }, false, "SwaggerSpec::Spec");
	}
	void Spec(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
};
