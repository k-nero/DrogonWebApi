#pragma once

#include <drogon/HttpController.h>
#include "MessageSeenByService.h"
#include <MessageSeenBy.h>
#include "ModelBinding.h"
#include "MessageSeenByModel.h"
#include "ObjToJson.h"


using namespace drogon;

namespace drogon
{
	template<>
	inline MessageSeenByModel fromRequest(const HttpRequest& req)
	{
		return bind_model<MessageSeenByModel>(req);
	}
};


class MessageSeenByController : public HttpController<MessageSeenByController>
{
public:
	static void initPathRouting()
	{
		registerMethod(&MessageSeenByController::Get, "/api/message-seen-by/{1}", { drogon::HttpMethod::Get }, false, "MessageSeenByController::Get"); // path is /IndexController
		registerMethod(&MessageSeenByController::GetPaginated, "/api/message-seen-by?page={1}&limit={2}", { drogon::HttpMethod::Get }, false, "MessageSeenByController::GetAll");
		registerMethod(&MessageSeenByController::Create, "/api/message-seen-by", { drogon::HttpMethod::Post }, false, "MessageSeenByController::Create");
		registerMethod(&MessageSeenByController::Update, "/api/message-seen-by/{1}", { drogon::HttpMethod::Patch }, false, "MessageSeenByController::Update");
		registerMethod(&MessageSeenByController::Delete, "/api/message-seen-by/{1}", { drogon::HttpMethod::Delete }, false, "MessageSeenByController::Delete");
	}
	// your declaration of processing function maybe like this:
	void Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, MessageSeenByModel model);
	void Update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id, MessageSeenByModel model);
	void Delete(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id);
	void GetPaginated(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, int page, int limit);
};
