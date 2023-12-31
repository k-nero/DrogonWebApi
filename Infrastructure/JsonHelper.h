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

template<class T> static Json::Value ToJson(T&t)
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
