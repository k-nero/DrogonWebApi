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
		K obj;
		obj = json.asInt();
		return obj;
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, unsigned int>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj;
		obj = json.asUInt();
		return obj;
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, long long>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj;
		obj = json.asInt64();
		return obj;
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, long>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj;
		obj = json.asInt64();
		return obj;
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, unsigned long>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj;
		obj = json.asInt64();
		return obj;
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, unsigned long long>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj;
		obj = json.asUInt64();
		return obj;
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, short>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj;
		obj = json.asInt();
		return obj;
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, unsigned short>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj;
		obj = json.asUInt();
		return obj;
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, float>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj;
		obj = json.asFloat();
		return obj;
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, double>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj;
		obj = json.asDouble();
		return obj;
	}

	template <class K = T, std::enable_if_t<is_string_v<K>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj;
		obj = json.asString();
		return obj;
	}

	template <class K = T, std::enable_if_t<std::is_same_v<K, bool>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj;
		obj = json.asBool();
		return obj;
	}

	template <class K = T, std::enable_if_t<is_shared_ptr_v<K>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj{};
		using U = typename K::element_type;
		obj = std::make_shared<U>(obj_from_json<U>(json));
		return obj;
	}

	template <class K = T, std::enable_if_t<is_weak_ptr_v<K>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj{};
		using U = typename K::element_type;
		obj = std::make_shared<U>(obj_from_json<U>(json));
		return obj;
	}

	template <class K = T, std::enable_if_t<is_vector_v<K>, bool> = true>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj{};
		using U = typename K::value_type;
		for (auto& j : json)
		{
			obj.push_back(obj_from_json<U>(j));
		}
		return obj;
	}

	template <class K = T, class D1 = boost::describe::describe_members<K, boost::describe::mod_any_access | boost::describe::mod_inherited>>
	static inline K obj_from_json(Json::Value& json)
	{
		K obj{};
		boost::mp11::mp_for_each<D1>([&](auto member)
		{
			obj.*(member).pointer = obj_from_json<std::remove_reference_t<decltype(obj.*(member).pointer)>>(json[member.name]);
		});
		return obj;
	}
};
