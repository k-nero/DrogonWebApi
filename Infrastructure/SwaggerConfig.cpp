#include "pch.h"
#include "SwaggerConfig.h"

void SwaggerConfig::SwaggerConfigurator::InitSwagger()
{
	auto json_str = CoreHelper::ReadTextFile("swagger.json");
	if (json_str.empty())
	{
		BOOST_LOG_TRIVIAL(error) << "swagger.json file not found";
		return;
	}
	swagger = CoreHelper::ParseJson(json_str);
}

void SwaggerConfig::SwaggerConfigurator::AddTag(const std::string& name, const std::string& description, ExternalDocs docs)
{
	Json::Value tag = Json::Value(Json::objectValue);
	tag["name"] = name;
	tag["description"] = description;

	if (!docs.description.empty() && !docs.url.empty())
	{
		tag["externalDocs"]["description"] = docs.description;
		tag["externalDocs"]["url"] = docs.url;
	}	
	swagger["tags"].append(tag);
}

