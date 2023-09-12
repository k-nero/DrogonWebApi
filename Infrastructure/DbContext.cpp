#include "pch.h"
#include "DbContext.h"

#include <iostream>
#include <ostream>
#include <SQLAPI.h>

DbContext::DbContext() = default;

SAConnection* DbContext::GetConnection()
{
	connection = new SAConnection();
	try
	{
		connection->Connect("192.168.50.24@CleanArchitecture", "sa", "12345", SA_SQLServer_Client);
		if (connection->isConnected())
		{
			//std::cout << "Connection success" << std::endl;
			return connection;
		}
		std::cout << "Connection failed" << std::endl;
	}
	catch (SAException& x)
	{
		std::cout << "There was an exception" << std::endl;
		std::cout << x.ErrText().GetMultiByteChars() << std::endl;
	/*	if (!connection->isAlive())
		{
			DbContext::~DbContext();
		}*/
	}
	return connection;
}

std::string DbContext::TestConnection() const
{
	SACommand cmd(connection, "SELECT @@version");
	cmd.Execute();
	while (cmd.FetchNext())
	{
		return cmd.Field(1).asString().GetMultiByteChars();
	}
	return {};
}

DbContext::~DbContext()
{
	if (connection->isConnected())
	{
		try
		{
			connection->Disconnect();
		}
		catch (SAException& x)
		{
			std::cout << "Unable to disconnect " << std::endl;
			std::cout << "Trying to destroy connection " << std::endl;
			std::cout << x.ErrText().GetMultiByteChars() << std::endl;
			connection->Destroy();
		}
	}
	if (connection != nullptr)
	{
		delete connection;
		connection = nullptr;
	}
}