#include "MessageController.h"

void MessageController::GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	try
	{
		auto task = std::future(std::async(std::launch::async, []() { return MessageService().GetAllMessages(); }));
		auto result(task.get());
		Json::Value rs = ObjToJson(result);
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

void MessageController::Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1)
{
	try
	{
		auto task = std::future(std::async(std::launch::async, [p1]() { return MessageService().GetMessageById(p1); }));
		std::shared_ptr<Message> result = task.get();
		Json::Value ret;
		if (result != nullptr)
		{
			ret = ObjToJson(result);
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

void MessageController::Create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, MessageModel model)
{
	try
	{
		auto task = std::future(std::async(std::launch::async, [&model]() { return MessageService().CreateMessage(model); }));
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
			resp->setStatusCode(k422UnprocessableEntity);
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

void MessageController::Update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id, MessageModel model)
{
	try
	{
		auto task = std::future(std::async(std::launch::async, [&model, &id]() { return MessageService().UpdateMessage(model, id); }));
		const auto rows_affected = task.get();
		if (rows_affected > 0)
		{
			Json::Value ret;
			ret["status"] = 200;
			ret["message"] = "Todo list updated successfully!";
			const auto resp = HttpResponse::newHttpJsonResponse(ret);
			resp->setStatusCode(k200OK);
			callback(resp);
			return;
		}
		else
		{
			Json::Value ret;
			ret["error"] = "Not Found";
			ret["status"] = 404;
			const auto resp = HttpResponse::newHttpJsonResponse(ret);
			resp->setStatusCode(k404NotFound);
			callback(resp);
			return;
		}
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

void MessageController::Delete(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id)
{
	try
	{
		auto task = std::future(std::async(std::launch::async, [&id]() { return MessageService().DeleteMessage(id); }));
		const auto rows_affected = task.get();
		if (rows_affected > 0)
		{
			Json::Value ret;
			ret["status"] = 200;
			ret["message"] = "Todo list deleted successfully!";
			const auto resp = HttpResponse::newHttpJsonResponse(ret);
			resp->setStatusCode(k200OK);
			callback(resp);
			return;
		}
		else
		{
			Json::Value ret;
			ret["error"] = "Not Found";
			ret["status"] = 404;
			const auto resp = HttpResponse::newHttpJsonResponse(ret);
			resp->setStatusCode(k404NotFound);
			callback(resp);
			return;
		}
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

void MessageController::GetPaginated(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string chat_id, int page, int limit)
{
	try
	{
		page == 0 ? page = 1 : page = page;
		limit == 0 ? limit = 30 : limit = limit;
		auto task = std::future(std::async(std::launch::async, [page, limit, chat_id]() { return MessageService().GetMessagesByChat(page, limit, chat_id); }));
		auto result = task.get();
		const auto resp = HttpResponse::newHttpJsonResponse(ObjToJson(result));
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
