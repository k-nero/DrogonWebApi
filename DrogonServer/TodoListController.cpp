#include "TodoListController.h"

void TodoListController::GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	try
	{
		TodoListService cmd;
		const std::vector<std::shared_ptr<TodoList>> todo_lists = cmd.GetAllTodoLists();
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

void TodoListController::Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1)
{

}
