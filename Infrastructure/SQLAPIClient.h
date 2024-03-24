#pragma once
#include "DbClientInterface.h"
#include <SQLAPI.h>
#include "DbContext.h"
#include "InfrastructureApi.h"
#include <typeinfo>
#include <type_traits>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <memory>


class INFRASTRUCTURE_API SQLAPIClient : public DbClientInterface
{
public:
	SQLAPIClient();
	~SQLAPIClient();

	void CreateCommand(const std::string& command_text) noexcept(false) override;
	void ExecuteCommand() noexcept(false) override;
	bool FetchNext() noexcept(false) override;
	std::shared_ptr<std::string> GetJsonStringResult(const std::string& column_name) noexcept(false) override;
	std::shared_ptr<std::string> GetJsonStringResult(const int column_index) noexcept(false) override;

	void BindParameter(const std::string& parameter_name, const std::string& value) noexcept(false) override;
	void BindParameter(const std::string& parameter_name, const int value) noexcept(false) override;
	void BindParameter(const std::string& parameter_name, const long value) noexcept(false) override;
	void BindParameter(const std::string& parameter_name, const bool value) noexcept(false) override;
	void BindParameter(const std::string& parameter_name, const double value) noexcept(false) override;
	void BindParameter(const std::string& parameter_name, const std::tm value) noexcept(false) override;
	void BindParameter(const std::string& parameter_name, const std::wstring value) noexcept(false) override;

	int GetIntResult(const std::string& column_name) noexcept(false) override;
	int GetIntResult(const int column_index) noexcept(false) override;
	long GetLongResult(const std::string& column_name) noexcept(false) override;
	long GetLongResult(const int column_index) noexcept(false) override;
	bool GetBoolResult(const std::string& column_name) noexcept(false) override;
	bool GetBoolResult(const int column_index) noexcept(false) override;
	double GetDoubleResult(const std::string& column_name) noexcept(false) override;
	double GetDoubleResult(const int column_index) noexcept(false) override;
	std::tm GetDateTimeResult(const std::string& column_name) noexcept(false) override;
	std::tm GetDateTimeResult(const int column_index) noexcept(false) override;
	std::string GetStringResult(const std::string& column_name) noexcept(false) override;
	std::string GetStringResult(const int column_index) noexcept(false) override;
protected:
	SAConnection* connection = nullptr;
	std::shared_ptr<SACommand> cmd;
};
