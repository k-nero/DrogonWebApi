#pragma once

#include <drogon/HttpController.h>
#include "TodoItemService.h"
#include <TodoItem.h>
#include <DbContext.h>
#include <memory.h>
#include "JsonHelper.h"
#include <boost/log/trivial.hpp>
#include "ModelBinding.h"


using namespace drogon;


namespace drogon
{
	template<>
	inline TodoItem fromRequest(const HttpRequest& req)
	{
		return from_request<TodoItem>(req);
	}
};

class TodoItemController : public HttpController<TodoItemController>
{
public:
	static void initPathRouting()
	{
		registerMethod(&TodoItemController::Get, "/api/todo-item/{1}", { drogon::HttpMethod::Get }, false, "TodoItemController::Get"); // path is /IndexController
		registerMethod(&TodoItemController::GetAll, "/api/todo-item", { drogon::HttpMethod::Get }, false, "TodoItemController::GetAll");
	}
	// your declaration of processing function maybe like this:
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
};
