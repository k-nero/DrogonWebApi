#pragma once
#include "InfrastructureApi.h"
#include "DbClientInterface.h"
#include "SQLAPIClient.h"
#include "SQLite3Client.h"


class INFRASTRUCTURE_API DbContext
{
public:
	DbContext() noexcept(false);

	[[nodiscard]] SAConnection * GetConnection();
	[[nodiscard]] std::string TestConnection();
	DbClientInterface* GetClient() const;
	~DbContext();

	private:
	Database connectionString;
};

