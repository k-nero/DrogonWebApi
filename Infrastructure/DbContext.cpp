#include "pch.h"
#include "DbContext.h"

#include <iostream>
#include <ostream>

DbContext::DbContext() 
{

}

SAConnection* DbContext::GetConnection() noexcept(false)
{
	if (ConfigProvider::GetInstance()->GetConnectionString().server.empty())
	{
		ConfigProvider::GetInstance()->Initialize();
	}
	connectionString = ConfigProvider::GetInstance()->GetConnectionString();

	auto connection = new SAConnection();
	try
	{

		connection->Connect(connectionString.server.c_str(), connectionString.username.c_str(), connectionString.password.c_str(), SA_SQLServer_Client);

		if (!connection->isConnected())
		{
			BOOST_LOG_TRIVIAL(fatal) << "Unable to connect to the database";
			throw std::exception("Unable to connect to the database");
		}
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



std::string DbContext::TestConnection() 
{
	auto connection = GetConnection();
	SACommand cmd(connection, "SELECT @@version");
	cmd.Execute();
	while (cmd.FetchNext())
	{
		return cmd.Field(1).asString().GetMultiByteChars();
	}
	return {};
}

DbClientInterface* DbContext::GetClient() const
{
	if (connectionString.type == "sqlserver")
	{
		return new SQLAPIClient();
	}
	else if (connectionString.type == "SQLite3")
	{
		return new SQLite3Client();
	}
}

DbContext::~DbContext()
{

}