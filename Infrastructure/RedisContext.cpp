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
	if (this->m_syncContext != nullptr)
	{
		return true;
	}

	std::string m_host = ConfigProvider::GetInstance()->GetRedisHost();
	std::string m_password = ConfigProvider::GetInstance()->GetRedisPassword();

	this->m_syncContext = redisConnectWithOptions(&opt);
	if (this->m_syncContext == nullptr || this->m_syncContext->err)
	{
		if (this->m_syncContext)
		{
			BOOST_LOG_TRIVIAL(error) << "Error: " << this->m_syncContext->errstr;
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "Can't allocate redis context";
		}
		return false;
	}
	redisReply* r = (redisReply*)redisCommand(this->m_syncContext, "AUTH %s", m_password.c_str());

	return true;
}

bool RedisContext::CreateSyncContext()
{
	redisOptions opt = { 0 };
	auto m_host = ConfigProvider::GetInstance()->GetRedisHost();
	do
	{
		(&opt)->type = REDIS_CONN_TCP; (&opt)->endpoint.tcp.ip = m_host.c_str(); (&opt)->endpoint.tcp.port = 6379;
	}
	while (0);
	opt.options = REDIS_OPT_PREFER_IP_UNSPEC;
	return RedisContext::CreateSyncContext(opt);
}

void RedisContext::SelectDb(int dbIndex)
{
	if (this->m_syncContext == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::SelectDb: m_syncContext is nullptr";
		return;
	}

	redisReply* reply = (redisReply*)redisCommand(this->m_syncContext, "SELECT %d", dbIndex);
	if (reply == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::SelectDb: redisCommand failed";
		return;
	}
	freeReplyObject(reply);
}

void RedisContext::SetString(const std::string& key, const std::string& value, int expireSeconds)
{
	if (this->m_syncContext == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::SetString: m_syncContext is nullptr";
		return;
	}


	redisReply* reply = (redisReply*)redisCommand(this->m_syncContext, "SET %s %b", key.c_str(), value.c_str(), value.length());
	if (reply == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::SetString: redisCommand failed";
		return;
	}

	if (expireSeconds > 0)
	{
		redisReply* reply = (redisReply*)redisCommand(this->m_syncContext, "EXPIRE %s %d", key.c_str(), expireSeconds);
		if (reply == nullptr)
		{
			BOOST_LOG_TRIVIAL(error) << "RedisContext::SetString: redisCommand failed";
			return;
		}
		freeReplyObject(reply);
	}
	freeReplyObject(reply);
}

void RedisContext::refreshTTL(const std::string& key, int expireSeconds)
{
	if (this->m_syncContext == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::refreshTTL: m_syncContext is nullptr";
		return;
	}

	redisReply* reply = (redisReply*)redisCommand(this->m_syncContext, "EXPIRE %s %d", key.c_str(), expireSeconds);
	if (reply == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::refreshTTL: redisCommand failed";
		return;
	}
	freeReplyObject(reply);
}

void RedisContext::RemoveKey(const std::string& key)
{
	if (this->m_syncContext == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::RemoveKey: m_syncContext is nullptr";
		return;
	}


	redisReply* reply = (redisReply*)redisCommand(this->m_syncContext, "DEL %s", key.c_str());
	if (reply == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::RemoveKey: redisCommand failed";
		return;
	}
	freeReplyObject(reply);
}

int RedisContext::GetNumOfDb()
{
	if (this->m_syncContext == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::GetNumOfDb: m_syncContext is nullptr";
		return 0;
	}

	redisReply* reply = (redisReply*)redisCommand(this->m_syncContext, "CONFIG GET databases");
	if (reply == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::GetNumOfDb: redisCommand failed";
		return 0;
	}
	int numOfDb = 0;

	if (reply->type == REDIS_REPLY_ARRAY)
	{
		numOfDb = reply->element[0]->integer;
		freeReplyObject(reply);
	}

	return numOfDb;
}

std::shared_ptr<std::string> RedisContext::GetString(const std::string& key)
{
	if (this->m_syncContext == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::GetString: m_syncContext is nullptr";
		return std::make_shared<std::string>("");
	}

	redisReply* reply = (redisReply*)redisCommand(this->m_syncContext, "GET %s", key.c_str());
	if (reply == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::GetString: redisCommand failed";
		return std::make_shared<std::string>("");
	}

	if (reply->type == REDIS_REPLY_NIL)
	{
		return std::make_shared<std::string>("");
	}

	auto value = std::make_shared<std::string>(reply->str);
	freeReplyObject(reply);
	return value;
}

std::vector<std::string> RedisContext::GetAllActiveKeys(std::string contain)
{

	if (this->m_syncContext == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::GetAllActiveKeys: m_syncContext is nullptr";
		return std::vector<std::string>();
	}

	//auto numofDb = RedisContext::GetNumOfDb();
	std::vector<std::string> keys;

	redisReply* reply = (redisReply*)redisCommand(this->m_syncContext, "KEYS *");
	if (reply == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::GetAllActiveKeys: redisCommand failed";
		return std::vector<std::string>();
	}

	for (int j = 0; j < reply->elements; j++)
	{
		if (!contain.empty())
		{
			if (strstr(reply->element[j]->str, contain.c_str()) != nullptr)
			{
				keys.push_back(reply->element[j]->str);
			}
		}
		else
		{
			keys.push_back(reply->element[j]->str);
		}
	}
	freeReplyObject(reply);
	return keys;
}
