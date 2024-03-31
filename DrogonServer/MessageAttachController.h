#pragma once

#include <drogon/HttpController.h>
#include "MessageAttachService.h"
#include <MessageAttach.h>
#include "ModelBinding.h"
#include "MessageAttachModel.h"
#include "ObjToJson.h"


using namespace drogon;

namespace drogon
{
	template<>
	inline MessageAttachModel fromRequest(const HttpRequest& req)
	{
		return bind_model<MessageAttachModel>(req);
	}
};


class MessageAttachController : public HttpController<MessageAttachController>
{
public:
	static void initPathRouting()
	{
		registerMethod(&MessageAttachController::Get, "/api/message-attach/{1}", { drogon::HttpMethod::Get }, false, "MessageAttachController::Get"); // path is /IndexController
		registerMethod(&MessageAttachController::GetPaginated, "/api/message-attach?page={1}&limit={2}", { drogon::HttpMethod::Get }, false, "MessageAttachController::GetAll");
		registerMethod(&MessageAttachController::Create, "/api/message-attach", { drogon::HttpMethod::Post }, false, "MessageAttachController::Create");
		registerMethod(&MessageAttachController::Update, "/api/message-attach/{1}", { drogon::HttpMethod::Patch }, false, "MessageAttachController::Update");
		registerMethod(&MessageAttachController::Delete, "/api/message-attach/{1}", { drogon::HttpMethod::Delete }, false, "MessageAttachController::Delete");
	}
	// your declaration of processing function maybe like this:
	void Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, MessageAttachModel model);
	void Update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id, MessageAttachModel model);
	void Delete(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id);
	void GetPaginated(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, int page, int limit);
};
