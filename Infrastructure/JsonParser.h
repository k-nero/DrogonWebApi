#pragma once
#include "TypeCheck.h"
#include <boost/describe.hpp>
#include <json/json.h>

template <class T>
class JsonParser
{
public:
	template <class K = T, std::enable_if_t<std::is_same_v<K, int>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{ 
		return json.asInt();
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, unsigned int>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		return json.asUInt();
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, long long>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		return json.asInt64();
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, long>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		return json.asInt64();
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, unsigned long>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		return json.asInt64();
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, unsigned long long>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{ 
		return json.asUInt64();
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, short>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		return json.asInt();
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, unsigned short>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		return json.asUInt();
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, float>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{ 
		return json.asFloat(); 
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, double>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		return json.asDouble();
	}

	template <class K = T, std::enable_if_t<is_string_v<K>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{ 
		return json.asString();
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, bool>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		return json.asBool();
	}

	template <class K = T, std::enable_if_t<is_shared_ptr_v<K>, bool> = true>
	static inline K obj_from_json( Json::Value& json)
	{
		return std::make_shared<K::element_type>(obj_from_json<K::element_type>(json));
	}

	template <class K = T, std::enable_if_t<is_weak_ptr_v<K>, bool> = true>
	static inline K obj_from_json( Json::Value& json)
	{
		return std::make_shared<K::element_type>(obj_from_json<K::element_type>(json));
	}

	template <class K = T, std::enable_if_t<is_vector_v<K>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj{};
		for (auto& j : json)
		{
			obj.push_back(std::move(obj_from_json<K::value_type>(j)));
		}
		return obj;
	}

	template <class K = T, class D1 = boost::describe::describe_members<K, boost::describe::mod_any_access | boost::describe::mod_inherited>>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj{};
		boost::mp11::mp_for_each<D1>([&](auto member)
		{
			if (json[member.name].isNull())
			{
				return;
			}
			obj.*(member).pointer = std::move(obj_from_json<std::remove_reference_t<decltype(obj.*(member).pointer)>>(json[member.name]));
		});
		return obj;
	}

	template <class K = T>
	static inline K obj_from_json(Json::Value&& json)
	{
		return obj_from_json<K>(std::forward<Json::Value&>(json));
	}
};
