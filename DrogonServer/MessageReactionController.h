#pragma once

#include <drogon/HttpController.h>
#include "MessageReactionService.h"
#include <MessageReaction.h>
#include "ModelBinding.h"
#include "MessageReactionModel.h"
#include "ObjToJson.h"


using namespace drogon;

namespace drogon
{
	template<>
	inline MessageReactionModel fromRequest(const HttpRequest& req)
	{
		return bind_model<MessageReactionModel>(req);
	}
};


class MessageReactionController : public HttpController<MessageReactionController>
{
public:
	static void initPathRouting()
	{
		registerMethod(&MessageReactionController::Get, "/api/message-reaction/{1}", { drogon::HttpMethod::Get }, false, "MessageReactionController::Get"); // path is /IndexController
		registerMethod(&MessageReactionController::GetPaginated, "/api/message-reaction?page={1}&limit={2}", { drogon::HttpMethod::Get }, false, "MessageReactionController::GetAll");
		registerMethod(&MessageReactionController::Create, "/api/message-reaction", { drogon::HttpMethod::Post }, false, "MessageReactionController::Create");
		registerMethod(&MessageReactionController::Update, "/api/message-reaction/{1}", { drogon::HttpMethod::Patch }, false, "MessageReactionController::Update");
		registerMethod(&MessageReactionController::Delete, "/api/message-reaction/{1}", { drogon::HttpMethod::Delete }, false, "MessageReactionController::Delete");
	}
	// your declaration of processing function maybe like this:
	void Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, MessageReactionModel model);
	void Update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id, MessageReactionModel model);
	void Delete(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id);
	void GetPaginated(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, int page, int limit);
};
