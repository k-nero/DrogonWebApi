#pragma once
#include <string>
#include <iostream>
#include <INIReader.h>

#ifdef INFRASTRUCTURE_EXPORTS
#define INFRASTRUCTURE_API __declspec(dllexport)
#else
#define INFRASTRUCTURE_API __declspec(dllimport)
#endif

struct INFRASTRUCTURE_API Database
{
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

	std::string GetPrivateRSAKey()
	{
		return privateRSAKey;
	}

	std::string GetPublicRSAKey()
	{
		return publicRSAKey;
	}
private:
	Database connectionString;
	std::string bcryptSecret;
	std::string privateRSAKey;
	std::string publicRSAKey;
};

