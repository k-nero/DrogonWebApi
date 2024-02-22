#include "TodoListController.h"

void TodoListController::GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	try
	{
		TodoListService cmd;
		auto task = std::future(std::async(std::launch::async, [&cmd]() { return cmd.GetAllTodoLists(); }));
		const auto todo_lists(task.get());
		Json::Value rs;
		for (const auto& todo_list : todo_lists)
		{
			rs.append(ToJson(todo_list));
		}
		const auto resp = HttpResponse::newHttpJsonResponse(rs);
		resp->setStatusCode(k200OK);
		callback(resp);
		return;
	}
	catch (std::exception& ex)
	{
		BOOST_LOG_TRIVIAL(error) << ex.what();
		Json::Value ret;
		ret["error"] = ex.what();
		ret["status"] = 500;
		const auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k500InternalServerError);
		callback(resp);
		return;
	}
	catch (...)
	{
		BOOST_LOG_TRIVIAL(error) << "Unknow exception";
		Json::Value ret;
		ret["error"] = "Internal Server Error";
		ret["status"] = 500;
		const auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k500InternalServerError);
		callback(resp);
		return;
	}
}

void TodoListController::Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1)
{
	try
	{
		TodoListService cmd;
		auto task = std::future(std::async(std::launch::async, [&cmd, p1]() { return cmd.GetTodoListById(p1); }));
		const auto todo_list = task.get();
		Json::Value ret;
		if (todo_list != nullptr)
		{
			ret = ToJson(todo_list);
		}
		else
		{
			ret["error"] = "Not Found";
			ret["status"] = 404;
			const auto resp = HttpResponse::newHttpJsonResponse(ret);
			resp->setStatusCode(k404NotFound);
			callback(resp);
			return;
		}
		const auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k200OK);
		callback(resp);
		return;
	}
	catch (std::exception& ex)
	{
		BOOST_LOG_TRIVIAL(error) << ex.what();
		Json::Value ret;
		ret["error"] = ex.what();
		ret["status"] = 500;
		const auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k500InternalServerError);
		callback(resp);
		return;
	}
	catch (...)
	{
		BOOST_LOG_TRIVIAL(error) << "Unknow exception";
		Json::Value ret;
		ret["error"] = "Internal Server Error";
		ret["status"] = 500;
		const auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k500InternalServerError);
		callback(resp);
		return;
	}
}

void TodoListController::Create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, TodoList todo_list)
{
	try
	{
		TodoListService cmd;
		auto task = std::future(std::async(std::launch::async, [&cmd, &todo_list]() { return cmd.CreateTodoList(&todo_list); }));
		const auto id = task.get();
		Json::Value ret;
		if (!id.empty())
		{
			ret["id"] = id;
			ret["status"] = 201;
			ret["message"] = "Todo list created successfully!";
		}
		else
		{
			ret["error"] = "Internal Server Error";
			ret["status"] = 500;
			const auto resp = HttpResponse::newHttpJsonResponse(ret);
			resp->setStatusCode(k500InternalServerError);
			callback(resp);
			return;
		}
		const auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k201Created);
		callback(resp);
		return;
	}
	catch (std::exception& ex)
	{
		BOOST_LOG_TRIVIAL(error) << ex.what();
		Json::Value ret;
		ret["error"] = ex.what();
		ret["status"] = 500;
		const auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k500InternalServerError);
		callback(resp);
		return;
	}
	catch (...)
	{
		BOOST_LOG_TRIVIAL(error) << "Unknow exception";
		Json::Value ret;
		ret["error"] = "Internal Server Error";
		ret["status"] = 500;
		const auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k500InternalServerError);
		callback(resp);
		return;
	}
}

void TodoListController::Update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, TodoList todo_list)
{
}

void TodoListController::Delete(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
}
