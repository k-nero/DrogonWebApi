#pragma once
#include <string>
#include <iostream>
#include <INIReader.h>
#include "InfrastructureApi.h"
#include <boost/log/trivial.hpp>


struct INFRASTRUCTURE_API Database
{
	std::string type;
	std::string server;
	std::string username;
	std::string password;
};

class INFRASTRUCTURE_API ConfigProvider
{
public:
	ConfigProvider();
	void Initialize();
	~ConfigProvider();
public:
	Database GetConnectionString()
	{
		return connectionString;
	}

	std::string GetBcryptSecret()
	{
		return bcryptSecret;
	}

	static ConfigProvider* GetInstance()
	{
		static ConfigProvider instance;
		return &instance;
	}

	/*static ConfigProvider& GetInstanceRef()
	{
		static ConfigProvider instance;
		return instance;
	}*/

	std::string GetPrivateRSAKey()
	{
		return privateRSAKey;
	}

	std::string GetPublicRSAKey()
	{
		return publicRSAKey;
	}

	std::string GetRedisHost()
	{
		return redis_host;
	}

	std::string GetRedisPassword()
	{
		return redis_password;
	}

private:
	Database connectionString;
	std::string bcryptSecret = "";
	std::string privateRSAKey = "";
	std::string publicRSAKey = "";
	std::string redis_host = "";
	std::string redis_password = "";
};

