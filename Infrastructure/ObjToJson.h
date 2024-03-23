#pragma once
#include "TypeCheck.h"
#include <boost/describe.hpp>
#include <json/json.h>

template <class T, std::enable_if_t<std::is_same_v<T, int>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::intValue;
	root = t;
	return root;
}

template <class T, std::enable_if_t<std::is_same_v<T, unsigned int>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::uintValue;
	root = t;
	return root;
}

template <class T, std::enable_if_t<std::is_same_v<T, long>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::intValue;
	root = t;
	return root;
}

template <class T, std::enable_if_t<std::is_same_v<T, unsigned long>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::uintValue;
	root = t;
	return root;
}

template <class T, std::enable_if_t<std::is_same_v<T, long long>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::intValue;
	root = t;
	return root;
}

template <class T, std::enable_if_t<std::is_same_v<T, unsigned long long>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::uintValue;
	root = t;
	return root;
}

template <class T, std::enable_if_t<std::is_same_v<T, bool>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::booleanValue;
	root = t;
	return root;
}

template <class T, std::enable_if_t<std::is_same_v<T, short>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::intValue;
	root = t;
	return root;
}

template <class T, std::enable_if_t<std::is_same_v<T, unsigned short>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::uintValue;
	root = t;
	return root;
}

template <class T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::realValue;
	root = t;
	return root;
}

template <class T, std::enable_if_t<is_string_v<T>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::stringValue;
	root = t;
	return root;
}

template <class T, std::enable_if_t<is_shared_ptr_v<T>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root;
	if (t)
	{
		root = Json::objectValue;
		root = ObjToJson(*t);
		return root;
	}
	root = Json::nullValue;
	return root;
}

template <class T, std::enable_if_t<is_weak_ptr_v<T>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root;
	if (t)
	{
		root = Json::objectValue;
		root = ObjToJson(*t);
		return root;
	}
	root = Json::nullValue;
	return root;
}

template <class T, std::enable_if_t<is_vector_v<T>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::arrayValue;
	for (auto& i : t)
	{
		root.append(ObjToJson(i));
	}
	return root;
}

template <class T, std::enable_if_t<is_map_v<T>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::objectValue;
	for (auto& i : t)
	{
		root[ObjToJson(i.first)] = ObjToJson(i.second);
	}
	return root;
}

template <class T, std::enable_if_t<is_list_v<T>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::arrayValue;
	for (auto& i : t)
	{
		root.append(ObjToJson(i));
	}
	return root;
}

template <class T, std::enable_if_t<std::is_pointer_v<T>, bool> = true>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root;
	if (t)
	{
		root = Json::objectValue;
		root = ObjToJson(*t);
		return root;
	}
	root = Json::nullValue;
	return root;
}

template <class T, class D1 = boost::describe::describe_members<T, boost::describe::mod_any_access | boost::describe::mod_inherited>>
static inline Json::Value ObjToJson(T& t)
{
	Json::Value root = Json::objectValue;
	boost::mp11::mp_for_each<D1>([&](auto member)
	{
		root[member.name] = ObjToJson(t.*(member).pointer);
	});

	return root;
}
