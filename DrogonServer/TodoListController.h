#pragma once

#include <drogon/HttpController.h>
#include "TodoListService.h"
#include <TodoList.h>
#include <DbContext.h>
#include <memory.h>
#include "JsonHelper.h"
#include <boost/log/trivial.hpp>


using namespace drogon;


class TodoListController : public HttpController<TodoListController>
{
public:
	static void initPathRouting()
	{
		registerMethod(&TodoListController::Get, "/api/todo-list/{1}", { drogon::HttpMethod::Get }, false, "IndexController::Get"); // path is /IndexController
		registerMethod(&TodoListController::GetAll, "/api/todo-lists", { drogon::HttpMethod::Get }, false, "IndexController::GetAll");
	}
	// your declaration of processing function maybe like this:
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
};
