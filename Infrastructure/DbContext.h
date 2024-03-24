#pragma once
#include <SQLAPI.h>
#include <INIReader.h>
#include "ConfigProvider.h"
#include <string>
#include "InfrastructureApi.h"
#include <boost/log/trivial.hpp>
#include <odbcAPI.h>
#include "DbClientInterface.h"
#include "SQLAPIClient.h"
#include "SQLite3Client.h"


class INFRASTRUCTURE_API DbContext
{
public:
	DbContext() noexcept(false);

	[[nodiscard]] SAConnection * GetConnection();

	[[nodiscard]] std::string TestConnection();

	DbClientInterface* GetClient();


	~DbContext();

	private:
	Database connectionString;
};

