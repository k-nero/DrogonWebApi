#include "pch.h"
#include "DbContext.h"

#include <iostream>
#include <ostream>
#include <SQLAPI.h>

DbContext::DbContext()
{

	if (ConfigProvider::GetInstance()->GetConnectionString().server.empty())
	{
		ConfigProvider::GetInstance()->Initialize();
	}
	connectionString = ConfigProvider::GetInstance()->GetConnectionString();
}

SAConnection* DbContext::GetConnection() throw (std::exception&)
{
	connection = new SAConnection();
	try
	{
		connection->Connect(connectionString.server.c_str(), connectionString.username.c_str(), connectionString.password.c_str(), SA_SQLServer_Client);
		if (connection->isConnected())
		{
			//std::cout << "Connection success" << std::endl;
			return connection;
		}
		std::cout << "Connection failed" << std::endl;
	}
	catch (SAException& x)
	{
		BOOST_LOG_TRIVIAL(fatal) << x.ErrText();
		BOOST_LOG_TRIVIAL(error) << x.ErrMessage();
#ifdef _DEBUG
		BOOST_LOG_TRIVIAL(debug) << x.ErrNativeCode();
		BOOST_LOG_TRIVIAL(debug) << x.ErrClass();
		BOOST_LOG_TRIVIAL(debug) << x.ErrPos();
#endif // DEBUG
		/*	if (!connection->isAlive())
			{
				DbContext::~DbContext();
			}*/
		throw std::exception(x.ErrText().GetMultiByteChars());
	}
	catch (std::exception& ex)
	{
		BOOST_LOG_TRIVIAL(fatal) << ex.what();
		throw ex;
	}
	catch (...)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Unknown exception";
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
			BOOST_LOG_TRIVIAL(fatal) << x.ErrText();
			BOOST_LOG_TRIVIAL(error) << x.ErrMessage();
#ifdef _DEBUG
			BOOST_LOG_TRIVIAL(debug) << x.ErrNativeCode();
			BOOST_LOG_TRIVIAL(debug) << x.ErrClass();
			BOOST_LOG_TRIVIAL(debug) << x.ErrPos();
#endif // DEBUG
			connection->Destroy();
		}
	}
	if (connection != nullptr)
	{
		delete connection;
		connection = nullptr;
	}
}