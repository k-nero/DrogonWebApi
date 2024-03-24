#pragma once

#include <string>

#include <memory>
#include "ConfigProvider.h"
#include "InfrastructureApi.h"
#include <boost/log/trivial.hpp>

class INFRASTRUCTURE_API DbClientInterface
{
public:
	virtual void CreateCommand(const std::string& command_text) noexcept(false) = 0;
	virtual void ExecuteCommand() noexcept(false) = 0;
	virtual bool FetchNext() noexcept(false) = 0;
	virtual void BindParameter(const std::string& parameter_name, const std::string& value) noexcept(false) = 0;
	virtual void BindParameter(const std::string& parameter_name, const int value) noexcept(false) = 0;
	virtual void BindParameter(const std::string& parameter_name, const long value) noexcept(false) = 0;
	virtual void BindParameter(const std::string& parameter_name, const long long value) noexcept(false) = 0;
	virtual void BindParameter(const std::string& parameter_name, const bool value) noexcept(false) = 0;
	virtual void BindParameter(const std::string& parameter_name, const double value) noexcept(false) = 0;
	virtual void BindParameter(const std::string& parameter_name, const std::tm value) noexcept(false) = 0;
	virtual void BindParameter(const std::string& parameter_name, const std::wstring value) noexcept(false) = 0;

	virtual int GetIntResult(const std::string& column_name) noexcept(false) = 0;
	virtual int GetIntResult(const int column_index) noexcept(false) = 0;
	virtual long long GetLongResult(const std::string& column_name) noexcept(false) = 0;
	virtual long long GetLongResult(const int column_index) noexcept(false) = 0;
	virtual bool GetBoolResult(const std::string& column_name) noexcept(false) = 0;
	virtual bool GetBoolResult(const int column_index) noexcept(false) = 0;
	virtual double GetDoubleResult(const std::string& column_name) noexcept(false) = 0;
	virtual double GetDoubleResult(const int column_index) noexcept(false) = 0;
	virtual std::tm GetDateTimeResult(const std::string& column_name) noexcept(false) = 0;
	virtual std::tm GetDateTimeResult(const int column_index) noexcept(false) = 0;
	virtual std::string GetStringResult(const std::string& column_name) noexcept(false) = 0;
	virtual std::string GetStringResult(const int column_index) noexcept(false) = 0;

	virtual void GetJsonStringResult(std::shared_ptr<std::string>& result ) noexcept(false) = 0;
	//virtual std::shared_ptr<Table> GetTableResult() noexcept(false) = 0;
	virtual void TestClient() noexcept(false) = 0;

};

typedef DbClientInterface DbClient;