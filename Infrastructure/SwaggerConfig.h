#pragma once
#include <string>
#include <vector>
#include <map>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <json/json.h>
#include "TypeCheck.h"
#include "CoreHelper.h"
#include "JsonParser.h"

namespace SwaggerConfig
{

	struct ExternalDocs
	{
		std::string description = "";
		std::string url = "";
	};

	class SwaggerConfigurator
	{
	public:
		SwaggerConfigurator() = default;
		~SwaggerConfigurator() = default;


		SwaggerConfigurator* GetInstance()
		{
			static SwaggerConfigurator instance;
			return &instance;
		}

		Json::Value& GetSwagger()
		{
			return swagger;
		}

		void InitSwagger();
		void AddTag(const std::string& name, const std::string& description, ExternalDocs docs = {});
		template <typename T>
		void AddDefinition()
		{
			T t;
			Json::Value definition = DefinitionBuilder(t);
			std::string object_name = std::string(typeid(T).name());
			object_name = object_name.substr(object_name.find_last_of(' ') + 1);
			swagger["definitions"][object_name] = definition;
		}

		void AddRoute();
		
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

	private:
		Json::Value swagger;
	};
};
