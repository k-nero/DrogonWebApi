#pragma once

#include <drogon/HttpController.h>
#include "ChatParticipantService.h"
#include <ChatParticipant.h>
#include "ModelBinding.h"
#include "ChatParticipantModel.h"
#include "ObjToJson.h"


using namespace drogon;

namespace drogon
{
	template<>
	inline ChatParticipantModel fromRequest(const HttpRequest& req)
	{
		return bind_model<ChatParticipantModel>(req);
	}
};


class ChatParticipantController : public HttpController<ChatParticipantController>
{
public:
	static void initPathRouting()
	{
		registerMethod(&ChatParticipantController::Get, "/api/chat-participant/{1}", { drogon::HttpMethod::Get }, false, "ChatParticipantController::Get"); // path is /IndexController
		registerMethod(&ChatParticipantController::GetPaginated, "/api/chat-participant?page={1}&limit={2}", { drogon::HttpMethod::Get }, false, "ChatParticipantController::GetAll");
		registerMethod(&ChatParticipantController::Create, "/api/chat-participant", { drogon::HttpMethod::Post }, false, "ChatParticipantController::Create");
		registerMethod(&ChatParticipantController::Update, "/api/chat-participant/{1}", { drogon::HttpMethod::Patch }, false, "ChatParticipantController::Update");
		registerMethod(&ChatParticipantController::Delete, "/api/chat-participant/{1}", { drogon::HttpMethod::Delete }, false, "ChatParticipantController::Delete");
	}
	// your declaration of processing function maybe like this:
	void Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, ChatParticipantModel model);
	void Update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id, ChatParticipantModel model);
	void Delete(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id);
	void GetPaginated(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, int page, int limit);
};
