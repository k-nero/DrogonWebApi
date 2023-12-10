#include "pch.h"
#include "ConfigProvider.h"
#include "CoreHelper.h"


ConfigProvider::ConfigProvider()
{
	
}

void ConfigProvider::Initialize()
{
	INIReader reader("config.ini");
	if (reader.ParseError() < 0)
	{
		std::cout << "Can't load 'config.ini'\n";
	}

	if (connectionString.server.empty())
	{
		connectionString.server = reader.Get("database", "server", "");
		connectionString.username = reader.Get("database", "username", "");
		connectionString.password = reader.Get("database", "password", "");
	}

	if (bcryptSecret.empty())
	{
		bcryptSecret = reader.Get("bcrypt", "secret", "default");
	}

	if (privateRSAKey.empty())
	{
		privateRSAKey = CoreHelper::ReadPemFile("private_key.pem");
	}
	if (publicRSAKey.empty())
	{
		publicRSAKey = CoreHelper::ReadPemFile("public_key.pem");
	}
}

ConfigProvider::~ConfigProvider()
{

}
