#pragma once
#include <SQLAPI.h>
#include <INIReader.h>
#include "ConfigProvider.h"
#include <string>
#include "InfrastructureApi.h"
#include <boost/log/trivial.hpp>
#include <odbcAPI.h>
  



class INFRASTRUCTURE_API DbContext
{
public:
	DbContext() noexcept(false);

	[[nodiscard]] SAConnection * GetConnection();

	[[nodiscard]] std::string TestConnection();


	~DbContext();

	private:
	Database connectionString;
};

