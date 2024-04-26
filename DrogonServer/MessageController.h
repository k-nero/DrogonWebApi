#pragma once

#include <drogon/HttpController.h>
#include "MessageService.h"
#include <Message.h>
#include "ModelBinding.h"
#include "MessageModel.h"
#include "ObjToJson.h"
#include <MediaMessageModel.h>


using namespace drogon;

namespace drogon
{
	template<>
	inline MessageModel fromRequest(const HttpRequest& req)
	{
		return bind_models<MessageModel>(req);
	}

	template<>
	inline MediaMessageModel fromRequest(const HttpRequest& req)
	{
		return bind_models<MediaMessageModel>(req);
	}
};


class MessageController : public HttpController<MessageController>
{
public:
	static void initPathRouting()
	{
		registerMethod(&MessageController::Get, "/api/message/{1}", { drogon::HttpMethod::Get }, false, "MessageController::Get"); // path is /IndexController
		registerMethod(&MessageController::GetPaginated, "/api/message?chat_id={1}&created_date={2}&page={3}&limit={4}", { drogon::HttpMethod::Get }, false, "MessageController::GetAll");
		registerMethod(&MessageController::Create, "/api/message", { drogon::HttpMethod::Post }, false, "MessageController::Create");
		registerMethod(&MessageController::CreateMedia, "/api/media-message", { drogon::HttpMethod::Post }, false, "MessageController::CreateMedia");
		registerMethod(&MessageController::Update, "/api/message/{1}", { drogon::HttpMethod::Patch }, false, "MessageController::Update");
		registerMethod(&MessageController::Delete, "/api/message/{1}", { drogon::HttpMethod::Delete }, false, "MessageController::Delete");
	}
	// your declaration of processing function maybe like this:
	void Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, MessageModel model);
	void CreateMedia(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, MediaMessageModel model);
	void Update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id, MessageModel model);
	void Delete(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id);
	void GetPaginated(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string chat_id, std::string created_date, int page, int limit );
};
