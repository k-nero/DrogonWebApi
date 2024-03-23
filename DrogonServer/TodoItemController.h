#pragma once

#include <drogon/HttpController.h>
#include "TodoItemService.h"
#include <TodoItem.h>
#include "ObjToJson.h"
#include "ModelBinding.h"
#include "TodoItemModel.h"

using namespace drogon;

namespace drogon
{
	template<>
	inline TodoItemModel fromRequest(const HttpRequest& req)
	{
		return bind_model<TodoItemModel>(req);
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
	void Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, TodoItemModel todo_item_model);
	void Update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1, TodoItemModel todo_item_model);
	void Delete(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
};
