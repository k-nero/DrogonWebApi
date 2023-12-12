#pragma once
#include <SQLAPI.h>
#include <INIReader.h>
#include "ConfigProvider.h"
#include <string>
#include "InfrastructureApi.h"

class INFRASTRUCTURE_API DbContext
{
public:
	DbContext();

	SAConnection * GetConnection();

	[[nodiscard]] std::string TestConnection() const;

	~DbContext();

	private:
	SAConnection * connection{};
	Database connectionString;
};

