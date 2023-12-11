#include "IndexController.h"


void IndexController::getAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	try
	{
		DbContext db;
		auto con = db.GetConnection();
		ApplicationUserCommand cmd(con);
		const std::vector<std::shared_ptr<ApplicationUser>> users = cmd.GetAllApplicationUsers();
		Json::Value rs;
		for (const auto& user : users)
		{
			rs.append(user->ToJson());
		}
		const auto resp = HttpResponse::newHttpJsonResponse(rs);
		resp->setStatusCode(k200OK);
		callback(resp);
		return;
	}
	catch (...)
	{
		Json::Value ret;
		ret["error"] = "Internal Server Error";
		ret["status"] = 500;
		const auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k500InternalServerError);
		callback(resp);
		return;

	}
}

void IndexController::get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string p1)
{
	try
	{
		DbContext db;
		const auto con = db.GetConnection();
		ApplicationUserCommand cmd(con);
		const std::shared_ptr<ApplicationUser> user(cmd.GetApplicationUserById(p1));
		Json::Value ret;
		if (user != nullptr)
		{
			ret = user->ToJson();
		}
		const auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k200OK);
		callback(resp);
		return;
	}
	catch (...)
	{
		Json::Value ret;
		ret["error"] = "Internal Server Error";
		ret["status"] = 500;
		const auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k500InternalServerError);
		callback(resp);
		return;
	}
}
