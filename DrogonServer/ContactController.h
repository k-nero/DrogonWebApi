#pragma once

#include <drogon/HttpController.h>
#include "ContactService.h"
#include <Contact.h>
#include "ModelBinding.h"
#include "ContactModel.h"
#include "ObjToJson.h"


using namespace drogon;

namespace drogon
{
	template<>
	inline ContactModel fromRequest(const HttpRequest& req)
	{
		return bind_model<ContactModel>(req);
	}
};


class ContactController : public HttpController<ContactController>
{
public:
	static void initPathRouting()
	{
		registerMethod(&ContactController::Get, "/api/contact/{1}", { drogon::HttpMethod::Get }, false, "ContactController::Get"); // path is /IndexController
		registerMethod(&ContactController::GetPaginated, "/api/contact?page={1}&limit={2}", { drogon::HttpMethod::Get }, false, "ContactController::GetAll");
		registerMethod(&ContactController::Create, "/api/contact", { drogon::HttpMethod::Post }, false, "ContactController::Create");
		registerMethod(&ContactController::Update, "/api/contact/{1}", { drogon::HttpMethod::Patch }, false, "ContactController::Update");
		registerMethod(&ContactController::Delete, "/api/contact/{1}", { drogon::HttpMethod::Delete }, false, "ContactController::Delete");
	}
	// your declaration of processing function maybe like this:
	void Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, ContactModel model);
	void Update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id, ContactModel model);
	void Delete(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id);
	void GetPaginated(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, int page, int limit);	
};
