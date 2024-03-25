#include "pch.h"
#include "DbContext.h"

DbContext::DbContext() 
{
	connectionString = ConfigProvider::GetInstance()->GetConnectionString();
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
	else
	{
		BOOST_LOG_TRIVIAL(fatal) << "Unknown database type";
		throw std::exception("Unknown database type");
	}
	
}

DbContext::~DbContext()
{

}