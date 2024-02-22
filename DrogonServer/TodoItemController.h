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
		registerMethod(&TodoItemController::Create, "/api/todo-item", { drogon::HttpMethod::Post }, false, "TodoItemController::Create");
		registerMethod(&TodoItemController::Update, "/api/todo-item/{1}", { drogon::HttpMethod::Patch }, false, "TodoItemController::Update");
		registerMethod(&TodoItemController::Delete, "/api/todo-item/{1}", { drogon::HttpMethod::Delete }, false, "TodoItemController::Delete");
		
	}
	// your declaration of processing function maybe like this:
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
	void Create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, TodoItem todo_item);
	void Update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1, TodoItem todo_item);
	void Delete(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
};
