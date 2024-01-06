#include "IndexController.h"


void IndexController::GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	try
	{
		DbContext db;
		auto con = db.GetConnection();
		TestApplicationUser cmd(con);
		const std::vector<std::shared_ptr<ApplicationUser>> users = cmd.GetAll();
		Json::Value rs;
		for (const auto& user : users)
		{
			rs.append(ToJson(*user));
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

void IndexController::Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string p1)
{
	try
	{
		DbContext db;
		const auto con = db.GetConnection();
		TestApplicationUser cmd(con);
		const std::shared_ptr<ApplicationUser> user(cmd.GetById(p1));
		Json::Value ret;
		if (user != nullptr)
		{
			ret = ToJson(*user);
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
