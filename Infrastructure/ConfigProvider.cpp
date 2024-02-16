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
		BOOST_LOG_TRIVIAL(error) << "Can't load 'config.ini'";
		BOOST_LOG_TRIVIAL(info) << "Failed to load config file in DrogonServer folder";

	}
	else
	{
		BOOST_LOG_TRIVIAL(info) << "Config file config.ini loaded !";
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
		if (bcryptSecret == "default")
		{
			BOOST_LOG_TRIVIAL(error) << "bcrypt secret is not set !";
			BOOST_LOG_TRIVIAL(info) << "Set bcrypt secret in config.ini with the example \n [bcrypt] \n secret = secret ";
		}
	}

	if (privateRSAKey.empty())
	{
		privateRSAKey = CoreHelper::ReadTextFile("private_key.pem");
		if (privateRSAKey.empty())
		{
			BOOST_LOG_TRIVIAL(error) << "private_key.pem is not set !";
			BOOST_LOG_TRIVIAL(info) << "Please provide a private RSA key in startup project folder ";

		}
	}
	if (publicRSAKey.empty())
	{
		publicRSAKey = CoreHelper::ReadTextFile("public_key.pem");
		if (publicRSAKey.empty())
		{
			BOOST_LOG_TRIVIAL(error) << "public_key.pem is not set !";
			BOOST_LOG_TRIVIAL(info) << "Please provide a public RSA key in startup project folder ";
		}
	}
}

ConfigProvider::~ConfigProvider()
{

}
