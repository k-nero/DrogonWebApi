#include "IndexController.h"


void IndexController::GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) 
{
	try
	{
		ApplicationUserService cmd;
		const std::vector<std::shared_ptr<ApplicationUser>> users = cmd.GetAllApplicationUsers();
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
	catch (std::exception& ex)
	{
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
		ApplicationUserService cmd;
		const std::shared_ptr<ApplicationUser> user(cmd.GetApplicationUserById(p1));
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
	catch (std::exception& ex)
	{
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
		Json::Value ret;
		ret["error"] = "Internal Server Error";
		ret["status"] = 500;
		const auto resp = HttpResponse::newHttpJsonResponse(ret);
		resp->setStatusCode(k500InternalServerError);
		callback(resp);
		return;
	}
}
