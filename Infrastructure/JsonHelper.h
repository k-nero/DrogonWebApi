#pragma once

#include "InfrastructureApi.h"
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <boost/json.hpp>
#include <json/json.h>

template <class T>
void tag_invoke(boost::json::value_from_tag const&, boost::json::value& v, const std::shared_ptr<T> p)
{
	if (p)
	{
		boost::json::value_from(*p, v);
	}
	else
	{
		v.emplace_null();
	}
}

template<class T, class D1 = boost::describe::describe_members<T, boost::describe::mod_any_access | boost::describe::mod_inherited>>
void tag_invoke(boost::json::value_from_tag const&, boost::json::value& v, T const& t)
{
	auto& obj = v.emplace_object();
	boost::mp11::mp_for_each<D1>([&](auto D)
	{
		obj[D.name] = boost::json::value_from(t.*D.pointer);
	});
}

template<class T> void extract(boost::json::object const& obj, char const* name, T& value)
{
	if(obj.find(name) == obj.end())
	{
		return;
	}
	value = boost::json::value_to<T>(obj.at(name));
}

template<class T> void extract(boost::json::object const& obj, char const* name, std::shared_ptr<T>& value)
{
	if (obj.find(name) == obj.end())
	{
		return;
	}
	value = std::make_shared<T>(boost::json::value_to<T>(obj.at(name)));
}

template<class T> void extract(boost::json::object const& obj, char const* name, std::vector<std::shared_ptr<T>>& value)
{
	if (obj.find (name) == obj.end())
	{
		return;
	}
	auto const& arr = obj.at(name).as_array();
	for (auto const& v : arr)
	{
		value.push_back(std::make_shared<T>(boost::json::value_to<T>(v)));
	}
}



template<class T, class D1 = boost::describe::describe_members<T, boost::describe::mod_any_access | boost::describe::mod_inherited>>
T tag_invoke(boost::json::value_to_tag<T> const&, boost::json::value const& v)
{
	auto const& obj = v.as_object();
	T t{};
	boost::mp11::mp_for_each<D1>([&](auto D)
	{
		extract(obj, D.name, t.*D.pointer);
	});
	return t;
}

template<class T>
std::shared_ptr<T> tag_invoke(boost::json::value_to_tag<std::shared_ptr<T>> const&, boost::json::value const& v)
{
	auto const& obj = v.as_object();
	std::shared_ptr<T> pt;
	pt = std::make_shared<T>(boost::json::value_to<T>(v));
	return pt;
}

template<class T>
std::vector<std::shared_ptr<T>> tag_invoke(boost::json::value_to_tag<std::vector<std::shared_ptr<T>>> const&, boost::json::value const& v)
{
	auto const& obj = v.as_array();
	std::vector<std::shared_ptr<T>> vt;
	
	for (auto const& v : obj)
	{
		std::shared_ptr<T> t = std::make_shared<T>(boost::json::value_to<T>(v));
		vt.push_back(t);
	}

	return vt;
}

template<class T>
static Json::Value ToJson(T& t)
{
	boost::json::value jv = boost::json::value_from(t);
	std::string json = boost::json::serialize(jv);
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
	}
	return root;
}