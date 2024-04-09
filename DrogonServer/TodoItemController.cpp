#include "TodoItemController.h"

void TodoItemController::GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	try
	{
		auto task = std::future(std::async(std::launch::async, []() { return TodoItemService().GetAllTodoItems(); }));
		auto result(task.get());
		const auto resp = HttpResponse::newHttpJsonResponse(obj_to_json(result));
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

void TodoItemController::Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1)
{
	try
	{
		auto task(std::async(std::launch::async, [p1]() { return TodoItemService().GetTodoItemById(p1); }));
		 auto todo_lists(task.get());
		Json::Value ret;
		if (todo_lists != nullptr)
		{
			ret = obj_to_json(todo_lists);
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

void TodoItemController::Create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, TodoItemModel todo_item_model)
{
	try
	{
		auto task(std::async(std::launch::async, [&todo_item_model]() { return TodoItemService().CreateTodoItem(todo_item_model); }));
		const auto id(task.get());
		Json::Value ret;
		ret["id"] = id;
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

void TodoItemController::Update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1, TodoItemModel todo_item_model)
{
	try
	{
		auto task(std::async(std::launch::async, [&todo_item_model, p1]() { return TodoItemService().UpdateTodoItem(todo_item_model, p1); }));
		const auto id(task.get());
		Json::Value ret;
		ret["id"] = id;
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

void TodoItemController::Delete(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1)
{
	try
	{
		auto task(std::async(std::launch::async, [p1]() { return TodoItemService().DeleteTodoItem(p1); }));
		const auto id(task.get());
		Json::Value ret;
		ret["id"] = id;
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
