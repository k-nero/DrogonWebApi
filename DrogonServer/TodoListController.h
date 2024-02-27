#pragma once

#include <drogon/HttpController.h>
#include "TodoListService.h"
#include <TodoList.h>
#include <DbContext.h>
#include <memory.h>
#include "JsonHelper.h"
#include <boost/log/trivial.hpp>
#include "ModelBinding.h"
#include "TodoListModel.h"


using namespace drogon;

namespace drogon
{
	template<>
	inline TodoListModel fromRequest(const HttpRequest& req)
	{
		return object_parser<TodoListModel>(req);
	}
};


class TodoListController : public HttpController<TodoListController>
{
public:
	static void initPathRouting()
	{
		registerMethod(&TodoListController::Get, "/api/todo-list/{1}", { drogon::HttpMethod::Get }, false, "TodoListController::Get"); // path is /IndexController
		registerMethod(&TodoListController::GetPaginated, "/api/todo-list?page={1}&limit={2}", { drogon::HttpMethod::Get }, false, "TodoListController::GetAll");
		registerMethod(&TodoListController::Create, "/api/todo-list", { drogon::HttpMethod::Post }, false, "TodoListController::Create");
		registerMethod(&TodoListController::Update, "/api/todo-list/{1}", { drogon::HttpMethod::Patch }, false, "TodoListController::Update");
		registerMethod(&TodoListController::Delete, "/api/todo-list/{1}", { drogon::HttpMethod::Delete }, false, "TodoListController::Delete");
	}
	// your declaration of processing function maybe like this:
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
	void Create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, TodoListModel todo_list_model);
	void Update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id, TodoListModel todo_list_model);
	void Delete(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id);
	void GetPaginated(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, int page = 1, int limit = 200);	
};
