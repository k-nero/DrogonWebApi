#include "MessageController.h"
#include <MessageAttachService.h>

void MessageController::GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	try
	{
		auto task = std::future(std::async(std::launch::async, []() { return MessageService().GetAllMessages(); }));
		auto result(task.get());
		Json::Value rs = obj_to_json(result);
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
			ret = obj_to_json(result);
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
			ret["message"] = "Message created successfully!";
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

void MessageController::CreateMedia(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, MediaMessageModel model)
{
	try
	{
		if (model.GetTextMessage().empty() && model.GetMedia().empty())
		{
			Json::Value ret;
			ret["error"] = "Text message or media message is required";
			ret["status"] = 422;
			const auto resp = HttpResponse::newHttpJsonResponse(ret);
			resp->setStatusCode(k422UnprocessableEntity);
			callback(resp);
			return;
		}

		auto message_model = MessageModel(model.GetTextMessage(), model.GetApplicationUserId(), model.GetChatRoomId(), model.GetQuoteMessageId());
		auto create_message = std::async(std::launch::async, [&message_model]() { return MessageService().CreateMessage(message_model); });
		const auto message_id = create_message.get();

		if (model.GetMedia().size() > 0)
		{
			for (auto& media : model.GetMedia())
			{
				media.SetMessageId(message_id);
				 MessageAttachService().CreateMessageAttach(media);
			}
		}

		Json::Value ret;
		if (!message_id.empty())
		{
			ret["id"] = message_id;
			ret["status"] = 201;
			ret["message"] = "Message created successfully!";
			const auto resp = HttpResponse::newHttpJsonResponse(ret);
			resp->setStatusCode(k201Created);
			callback(resp);
			return;
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
			ret["message"] = "Message updated successfully!";
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
			ret["message"] = "Message deleted successfully!";
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

void MessageController::GetPaginated(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string chat_id, std::string created_date ,int page, int limit)
{
	try
	{
		page == 0 ? page = 1 : page = page;
		limit == 0 ? limit = 30 : limit = limit;
		auto task = std::future(std::async(std::launch::async, [page, limit, chat_id, created_date]()
		{ 
			if (!chat_id.empty() && !created_date.empty())
			{
				return MessageService().GetMessagesByDate(page, limit, chat_id, created_date);
			}
			else if (!chat_id.empty())
			{
				return MessageService().GetMessagesByChat(page, limit, chat_id);
			}
			else
			{
				return MessageService().GetMessagesByPage(page, limit);
			}
		}));
		auto result = task.get();
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
