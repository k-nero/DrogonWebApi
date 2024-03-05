#include "SwaggerSpec.h"

void SwaggerSpec::Spec(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback)
{
	try
	{
		std::string json = CoreHelper::ReadTextFile("swagger/openapi.json");
		Json::Value root;
		Json::CharReaderBuilder builder;
		Json::CharReader* reader = builder.newCharReader();
		std::string errors;
		bool parsingSuccessful = reader->parse(json.c_str(), json.c_str() + json.size(), &root, &errors);
		delete reader;
		if (!parsingSuccessful)
		{
			root = Json::Value();
			root["error"] = errors;
			auto resp = HttpResponse::newHttpJsonResponse(root);
			resp->setStatusCode(k400BadRequest);
			callback(resp);
		}

		auto resp = HttpResponse::newHttpJsonResponse(root);
		resp->setStatusCode(k200OK);
		callback(resp);

	}
	catch (const std::exception& e)
	{
		BOOST_LOG_TRIVIAL(error) << e.what();
	}
	catch (...)
	{
		BOOST_LOG_TRIVIAL(error) << "Unknown error";
	}
}
