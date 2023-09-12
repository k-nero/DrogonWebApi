#pragma once
#include <SQLAPI.h>
#include <INIReader.h>
#include "ConfigProvider.h"
#include <string>


class __declspec(dllexport) DbContext
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

