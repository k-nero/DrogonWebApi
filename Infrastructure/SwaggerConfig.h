#pragma once
#include <string>
#include <vector>
#include <map>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <json/json.h>
#include "TypeCheck.h"

namespace SwaggerConfig
{

	struct Contact
	{
		std::string email = " ";
	};

	struct License
	{
		std::string name = "MIT";
		std::string url = "";
	};
	struct Info
	{
		std::string description = "Swagger API";
		std::string version = "1.0.0";
		std::string title = "Swagger API";
		std::string termsOfService = "http://swagger.io/terms/";
		Contact contact;
		License license;
	};
	struct ExternalDocs
	{
		std::string description = "";
		std::string url = "";
	};

	struct Tags
	{
		std::string name = " ";
		std::string description = " ";
		ExternalDocs externalDocs;
	};

	struct Swagger
	{
		std::string swagger = "2.0";
		std::string host = "127.0.0.1";
		std::string basePath = "/";
		std::vector<std::string> schemes = { "http", "https"};
		Info info;
		std::vector<Tags> tags;
		
	};

	struct Parameter
	{
		std::string name = "";
		std::string in = "";
		std::string description = "";
		bool required = false;
		std::string type = "";
	};
	
	struct Method
	{
		std::vector<Tags> tags;
		std::string summary = "";
		std::string description = "";
		std::string operationId = "";
		std::vector<std::string> comsumes = { "application/json", "application/xml" };
		std::vector<std::string> produces = { "application/json", "application/xml" };
		std::vector<Parameter> parameters;
	};

	struct Response
	{
		std::string description = "";
	};

	struct Responses
	{
		std::string description = "";
		std::map<std::string, Response> responses;
	};

	class SwaggerConfigurator
	{
	public:
		SwaggerConfigurator() = default;
		~SwaggerConfigurator() = default;

		template <typename T, class D = boost::describe::describe_members<T, boost::describe::mod_any_access | boost::describe::mod_inherited> >
		Json::Value DefinitionBuilder(T& t)
		{
			Json::Value definition = Json::Value(Json::objectValue);
			definition["type"] = "object";
			definition["properties"] = Json::Value(Json::objectValue);

			boost::mp11::mp_for_each<D>([&](auto member)
			{
				if (std::is_same<std::remove_reference_t<decltype(t.*(member).pointer)>, int>::value)
				{
					definition["properties"][member.name]["type"] = "integer";
					definition["properties"][member.name]["format"] = "int64";
				}
				else if (std::is_same<std::remove_reference_t<decltype(t.*(member).pointer)>, bool>::value)
				{
					definition["properties"][member.name]["type"] = "boolean";
				}
				else if (std::is_same<std::remove_reference_t<decltype(t.*(member).pointer)>, double>::value
					  || std::is_same<std::remove_reference_t<decltype(t.*(member).pointer)>, float>::value)
				{
					definition["properties"][member.name]["type"] = "number";
					definition["properties"][member.name]["format"] = "double";
				}
				else if (std::is_same<std::remove_reference_t<decltype(t.*(member).pointer)>, std::string>::value
					  || std::is_same<std::remove_reference_t<decltype(t.*(member).pointer)>, std::wstring>::value
					  || std::is_same<std::remove_reference_t<decltype(t.*(member).pointer)>, std::string_view>::value
					  || std::is_same<std::remove_reference_t<decltype(t.*(member).pointer)>, std::wstring_view>::value)
				{
					definition["properties"][member.name]["type"] = "string";
				}
				else if (std::is_same<std::remove_reference_t<decltype(t.*(member).pointer)>, std::tm>::value)
				{
					/*definition["properties"][member.name]["type"] = "string";
					definition["properties"][member.name]["format"] = "date-time";*/
				}
				else if (is_vector_v<std::remove_reference_t<decltype(t.*(member).pointer)>>)
				{
					definition["properties"][member.name]["type"] = "array";
					using Type = std::remove_reference_t<decltype(t.*(member).pointer)>;
					using inner_type = has_value_type_t<Type>;
					using inner_elem_type = std::remove_pointer_t<has_element_type_t<std::remove_reference_t<inner_type>>>;
					std::string object_name = std::string(typeid(inner_elem_type).name());
					object_name = object_name.substr(object_name.find_last_of(' ') + 1);
					definition["properties"][member.name]["items"] = "#/definitions/" + object_name;
				}
				else if (is_shared_ptr_v<std::remove_reference_t<decltype(t.*(member).pointer)>>)
				{
					definition["properties"][member.name]["type"] = "object";
					using Type = std::remove_reference_t<decltype(t.*(member).pointer)>;
					using inner_elem_type = std::remove_pointer_t<has_element_type_t<std::remove_reference_t<Type>>>;
					std::string object_name = std::string(typeid(inner_elem_type).name());
					object_name = object_name.substr(object_name.find_last_of(' ') + 1);
					definition["properties"][member.name]["$ref"] = "#/definitions/" + object_name;
				}
			});
			return definition;
		}
	};
};
