#pragma once

#include <drogon/HttpController.h>
#include "ChatRoomService.h"
#include <ChatRoom.h>
#include "ModelBinding.h"
#include "ChatRoomModel.h"
#include "ObjToJson.h"


using namespace drogon;

namespace drogon
{
	template<>
	inline ChatRoomModel fromRequest(const HttpRequest& req)
	{
		return bind_model<ChatRoomModel>(req);
	}
};


class ChatRoomController : public HttpController<ChatRoomController>
{
public:
	static void initPathRouting()
	{
		registerMethod(&ChatRoomController::Get, "/api/chat-room/{1}", { drogon::HttpMethod::Get }, false, "ChatRoomController::Get"); // path is /IndexController
		registerMethod(&ChatRoomController::GetPaginated, "/api/chat-room?page={1}&limit={2}", { drogon::HttpMethod::Get }, false, "ChatRoomController::GetAll");
		registerMethod(&ChatRoomController::Create, "/api/chat-room", { drogon::HttpMethod::Post }, false, "ChatRoomController::Create");
		registerMethod(&ChatRoomController::Update, "/api/chat-room/{1}", { drogon::HttpMethod::Patch }, false, "ChatRoomController::Update");
		registerMethod(&ChatRoomController::Delete, "/api/chat-room/{1}", { drogon::HttpMethod::Delete }, false, "ChatRoomController::Delete");
	}
	// your declaration of processing function maybe like this:
	void Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, ChatRoomModel model);
	void Update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id, ChatRoomModel model);
	void Delete(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id);
	void GetPaginated(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, int page, int limit);
};
