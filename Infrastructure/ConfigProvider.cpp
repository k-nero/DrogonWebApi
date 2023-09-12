#include "pch.h"
#include "ConfigProvider.h"

ConfigProvider::ConfigProvider()
{
	
}

void ConfigProvider::InitDataBase()
{
	if (connectionString.server.empty())
	{
		INIReader reader("config.ini");
		if (reader.ParseError() < 0)
		{
			std::cout << "Can't load 'config.ini'\n";
		}

		connectionString.server = reader.Get("database", "server", "");
		connectionString.username = reader.Get("database", "username", "");
		connectionString.password = reader.Get("database", "password", "");
	}
}

ConfigProvider::~ConfigProvider()
{

}
