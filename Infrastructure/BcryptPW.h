#pragma once
#include <windows.h>
#include <bcrypt.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <boost/beast/core/detail/base64.hpp>
#include "ConfigProvider.h"	
#include "InfrastructureApi.h"
#include <boost/log/trivial.hpp>

class INFRASTRUCTURE_API Bcrypt
{
public:
	Bcrypt();
	~Bcrypt();
	static std::string HashPassword(const std::string& password) noexcept(false);
	static bool VerifyPassword(std::string password, std::string hash) noexcept(false);
private:
};

