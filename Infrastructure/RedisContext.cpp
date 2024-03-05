#include "pch.h"
#include "RedisContext.h"

RedisContext::~RedisContext()
{
	if (this->m_syncContext != nullptr)
	{
		redisFree(this->m_syncContext);
	}

	if (this->m_asyncContext != nullptr)
	{
		redisAsyncDisconnect(this->m_asyncContext);
		redisAsyncFree(this->m_asyncContext);
	}
}

bool RedisContext::CreateSyncContext(redisOptions opt)
{
    return false;
}

bool RedisContext::CreateSyncContext()
{

	redisOptions opt = { 0 };



	if (this->m_syncContext != nullptr)
	{
		return true;
	}

	std::string m_host = ConfigProvider::GetInstance()->GetRedisHost();
	std::string m_password = ConfigProvider::GetInstance()->GetRedisPassword();

	this->m_syncContext = redisConnect(m_host.c_str(), 6379);
	if (this->m_syncContext == nullptr || this->m_syncContext->err)
	{
		if (this->m_syncContext)
		{
			printf("Error: %s\n", this->m_syncContext->errstr);
		}
		else
		{
			printf("Can't allocate redis context\n");
		}
		return false;
	}

	return true;
}
