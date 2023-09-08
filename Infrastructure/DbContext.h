#pragma once
#include <SQLAPI.h>
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
};

