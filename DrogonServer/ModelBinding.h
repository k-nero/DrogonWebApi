#pragma once
#include <drogon/HttpRequest.h>
#include <boost/describe.hpp>

namespace drogon
{
	template <typename T, class D = boost::describe::describe_members<T, boost::describe::mod_any_access | boost::describe::mod_inherited> >
	inline T bind_model(const HttpRequest& req)
	{
		T obj{};
		std::shared_ptr<Json::Value> json = req.getJsonObject();
		if (json)
		{
			boost::mp11::mp_for_each<D>([&](auto member)
			{
				if (member.name == "Id" || member.name == "id")
				{
					return;
				}
				if (std::is_same<std::remove_reference_t<decltype(obj.*(member).pointer)>, int>::value)
				{
					(long&)(obj.*(member).pointer) = (*json)[member.name].asInt();
				}
				else if (std::is_same<std::remove_reference_t<decltype(obj.*(member).pointer)>, bool>::value)
				{
					(bool&)(obj.*(member).pointer) = (*json)[member.name].asBool();
				}
				else if (std::is_same<std::remove_reference_t<decltype(obj.*(member).pointer)>, double>::value
					|| std::is_same<std::remove_reference_t<decltype(obj.*(member).pointer)>, float>::value)
				{
					(double&)(obj.*(member).pointer) = (*json)[member.name].asDouble();
				}
				else if (std::is_same<std::remove_reference_t<decltype(obj.*(member).pointer)>, std::string>::value
					|| std::is_same<std::remove_reference_t<decltype(obj.*(member).pointer)>, std::wstring>::value
					|| std::is_same<std::remove_reference_t<decltype(obj.*(member).pointer)>, std::string_view>::value
					|| std::is_same<std::remove_reference_t<decltype(obj.*(member).pointer)>, std::wstring_view>::value)
				{
					if ((*json)[member.name].isNull())
					{
						(std::string&)(obj.*(member).pointer) = "";
					}
					else
					{
						(std::string&)(obj.*(member).pointer) = (*json)[member.name].asString();
					}
				}
				else if (std::is_same<std::remove_reference_t<decltype(obj.*(member).pointer)>, std::tm>::value)
				{

				}
			});
		}

		return obj;
	}

	template <typename T, class D = boost::describe::describe_members<T, boost::describe::mod_any_access | boost::describe::mod_inherited> >
	static inline T bind_models(const HttpRequest& req)
	{
		T obj{};
		std::shared_ptr<Json::Value> json = req.getJsonObject();
		if (json)
		{
			return JsonParser<T>::obj_from_json(*json);
		}
		return obj;
	}

	/*template<>
	inline TodoList fromRequest(const HttpRequest& req)
	{
		TodoList obj{};
		std::shared_ptr<Json::Value> json = req.getJsonObject();
		if (json)
		{
			if (json->isMember("Title"))
			{
				obj.SetTitle(json->get("Title", "").asString());
			}
			if (json->isMember("Description"))
			{
				obj.SetDescription(json->get("Description", "").asString());
			}
		}
		return obj;
	}*/
}