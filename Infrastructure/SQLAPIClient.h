#pragma once
#include "DbClientInterface.h"
#include <SQLAPI.h>
#include "InfrastructureApi.h"
#include "ConfigProvider.h"


class INFRASTRUCTURE_API SQLAPIClient : public DbClientInterface
{
public:
	SQLAPIClient();
	~SQLAPIClient() override;

	void CreateCommand(const std::string& command_text) noexcept(false) override;
	void ExecuteCommand() noexcept(false) override;
	bool FetchNext() noexcept(false) override;
	void GetJsonStringResult(std::shared_ptr<std::string>& result) noexcept(false) override;

	void BindParameter(const std::string& parameter_name, const std::string& value) noexcept(false) override;
	void BindParameter(const std::string& parameter_name, const int value) noexcept(false) override;
	void BindParameter(const std::string& parameter_name, const long value) noexcept(false) override;
	void BindParameter(const std::string& parameter_name, const long long value) noexcept(false) override;
	void BindParameter(const std::string& parameter_name, const bool value) noexcept(false) override;
	void BindParameter(const std::string& parameter_name, const double value) noexcept(false) override;
	void BindParameter(const std::string& parameter_name, const std::tm value) noexcept(false) override;
	void BindParameter(const std::string& parameter_name, const std::wstring value) noexcept(false) override;

	int GetIntResult(const std::string& column_name) noexcept(false) override;
	int GetIntResult(const int column_index) noexcept(false) override;
	long long GetLongResult(const std::string& column_name) noexcept(false) override;
	long long GetLongResult(const int column_index) noexcept(false) override;
	bool GetBoolResult(const std::string& column_name) noexcept(false) override;
	bool GetBoolResult(const int column_index) noexcept(false) override;
	double GetDoubleResult(const std::string& column_name) noexcept(false) override;
	double GetDoubleResult(const int column_index) noexcept(false) override;
	std::tm GetDateTimeResult(const std::string& column_name) noexcept(false) override;
	std::tm GetDateTimeResult(const int column_index) noexcept(false) override;
	std::string GetStringResult(const std::string& column_name) noexcept(false) override;
	std::string GetStringResult(const int column_index) noexcept(false) override;
	long long AffectedRows() noexcept(false) override;
	void TestClient() noexcept(false) override;

protected:
	SAConnection* connection = nullptr;
	std::shared_ptr<SACommand> cmd;
};
