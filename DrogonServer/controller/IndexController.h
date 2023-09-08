#pragma once

#include <drogon/HttpController.h>
#include <ApplicationUserCommand.h>
#include <ApplicationUser.h>
#include <DbContext.h>
#include <memory.h>

using namespace drogon;

class IndexController : public HttpController<IndexController>
{
public:
	static void initPathRouting()
	{
		// use METHOD_ADD to add your custom processing function here;
		METHOD_ADD( IndexController::get, "/{1}", Get ); // path is /IndexController
		METHOD_ADD(IndexController::getAll, "", Get);
		// METHOD_ADD(IndexController::your_method_name, "/{1}/{2}/list", Get); // path is /IndexController/{arg1}/{arg2}/list
		// ADD_METHOD_TO(IndexController::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
	}
	// your declaration of processing function maybe like this:
	void getAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void get( const HttpRequestPtr& req, std::function<void( const HttpResponsePtr& )>&& callback, std::string p1 );
	// void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
};
