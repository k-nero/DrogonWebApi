#pragma once
#include <SQLAPI.h>
#include <vector>
#include "CoreHelper.h"
#include "ApplicationApi.h"
#include <boost/describe.hpp>
#include <boost/mp11.hpp>

template<typename T>
__interface IBaseCommand
{
	virtual std::shared_ptr<T> GetById(const std::string& id) = 0;
	virtual std::vector<std::shared_ptr<T>>GetAll(std::string query = "") = 0;
	virtual int Create(T* item) = 0;
	virtual int Update(T* item) = 0;
	template<class D1 = boost::describe::describe_members<T, boost::describe::mod_any_access | boost::describe::mod_inherited>>
	virtual std::shared_ptr<T> GetFromCommand(SACommand& cmd) = 0;
};