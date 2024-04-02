#pragma once

#include <drogon/HttpController.h>
#include "ContactListService.h"
#include <ContactList.h>
#include "ModelBinding.h"
#include "ContactListModel.h"
#include "ObjToJson.h"


using namespace drogon;

namespace drogon
{
	template<>
	inline ContactListModel fromRequest(const HttpRequest& req)
	{
		return bind_model<ContactListModel>(req);
	}
};


class ContactListController : public HttpController<ContactListController>
{
public:
	static void initPathRouting()
	{
		registerMethod(&ContactListController::Get, "/api/contact-list/{1}", { drogon::HttpMethod::Get }, false, "ContactListController::Get"); // path is /IndexController
		registerMethod(&ContactListController::GetPaginated, "/api/contact-list?page={1}&limit={2}&user_id={3}", { drogon::HttpMethod::Get }, false, "ContactListController::GetAll");
		registerMethod(&ContactListController::Create, "/api/contact-list", { drogon::HttpMethod::Post }, false, "ContactListController::Create");
		registerMethod(&ContactListController::Update, "/api/contact-list/{1}", { drogon::HttpMethod::Patch }, false, "ContactListController::Update");
		registerMethod(&ContactListController::Delete, "/api/contact-list/{1}", { drogon::HttpMethod::Delete }, false, "ContactListController::Delete");
	}
	// your declaration of processing function maybe like this:
	void Get(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string p1);
	void GetAll(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
	void Create(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, ContactListModel model);
	void Update(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id, ContactListModel model);
	void Delete(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, const std::string id);
	void GetPaginated(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, int page, int limit, std::string user_id);
};
