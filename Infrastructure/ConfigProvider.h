#pragma once
#include <string>
#include <iostream>
#include <INIReader.h>

struct Database
{
	std::string server;
	std::string username;
	std::string password;
};

class ConfigProvider
{
public:
	ConfigProvider();
	void InitDataBase();
	~ConfigProvider();
public:
	Database GetConnectionString()
	{
		return connectionString;
	}

	static ConfigProvider* GetInstance()
	{
		static ConfigProvider instance;
		return &instance;
	}
private:
	Database connectionString;
};

