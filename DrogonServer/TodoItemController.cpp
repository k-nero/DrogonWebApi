#include "TodoItemController.h"

void TodoItemController::GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	try
	{
		TodoItemService cmd;
		const std::vector<std::shared_ptr<TodoItem>> todo_lists = cmd.GetAllTodoItems();
		Json::Value rs;
		for (const auto& todo_list : todo_lists)
		{
			rs.append(ToJson(*todo_list));
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

void TodoItemController::Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1)
{
	try
	{
		TodoItemService cmd;
		const std::shared_ptr<TodoItem> todo_lists(cmd.GetTodoItemById(p1));
		Json::Value ret;
		if (todo_lists != nullptr)
		{
			ret = ToJson(*todo_lists);
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
