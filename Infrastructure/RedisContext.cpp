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
	if(this->m_syncContext != nullptr)
	{
		return true;
	}
	timeval timeout{ 1, 500000 }; // 1.5 seconds
	redisOptions opt = { 0 };
	auto m_host = ConfigProvider::GetInstance()->GetRedisHost();
	opt.type = REDIS_CONN_TCP;
	opt.endpoint.tcp.ip = m_host.c_str();
	opt.endpoint.tcp.port = 6379;
	opt.connect_timeout = &timeout;
	opt.options |= REDIS_OPT_PREFER_IP_UNSPEC | REDIS_OPT_REUSEADDR;
	return RedisContext::CreateSyncContext(opt);
}

void RedisContext::SelectDb(int dbIndex)
{
	std::unique_lock lock(mutex_);
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
	std::unique_lock lock(mutex_);
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
	std::unique_lock lock(mutex_);
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
	std::unique_lock lock(mutex_);
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

long long RedisContext::GetNumOfDb()
{
	std::unique_lock lock(mutex_);
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
	long long numOfDb = 0;

	if (reply->type == REDIS_REPLY_ARRAY)
	{
		numOfDb = reply->element[0]->integer;
		freeReplyObject(reply);
	}

	return numOfDb;
}

std::shared_ptr<std::string> RedisContext::GetString(const std::string& key)
{
	std::unique_lock lock(mutex_);
	if (this->m_syncContext == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::GetString: m_syncContext is nullptr";
		return nullptr;
	}

	redisReply* reply = (redisReply*)redisCommand(this->m_syncContext, "GET %s", key.c_str());
	if (reply == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::GetString: redisCommand failed";
		BOOST_LOG_TRIVIAL(error) << "Error: " << this->m_syncContext->errstr;
		return nullptr;
	}

	if (reply->type == REDIS_REPLY_NIL)
	{
		return nullptr;
	}
	if (reply->str == nullptr)
	{
		return nullptr;
	}
	auto value = std::make_shared<std::string>(reply->str);
	freeReplyObject(reply);
	return value;
}

std::vector<std::string> RedisContext::GetAllActiveKeys(std::string contain)
{
	std::unique_lock lock(mutex_);
	if (this->m_syncContext == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::GetAllActiveKeys: m_syncContext is nullptr";
		return std::vector<std::string>();
	}

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

int RedisContext::RemoveKeyContaining(const std::string& key_word)
{
	if (this->m_syncContext == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "Redis context is nullptr";
		return 0;
	}

	auto keys = this->GetAllActiveKeys();
	int count = 0;
	for (auto& key : keys)
	{
		if (key.find(key_word) != std::string::npos)
		{
			this->RemoveKey(key);
			count++;
		}
	}
	return count;
}

void RedisContext::SetStringAsync(const std::string& key, const std::string& value, int expireSeconds)
{
	if (this->m_asyncContext == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::SetStringAsync: m_asyncContext is nullptr";
		return;
	}

	redisAsyncCommand(this->m_asyncContext, [](redisAsyncContext* c, void* r, void* privdata)
	{
		if (r == nullptr)
		{
			BOOST_LOG_TRIVIAL(error) << "RedisContext::SetStringAsync: redisAsyncCommand failed";
			return;
		}
		freeReplyObject((redisReply*)r);
	}, nullptr, "SET %s %b", key.c_str(), value.c_str(), value.length());
}

void RedisContext::refreshTTLAsync(const std::string& key, int expireSeconds)
{
	if (this->m_asyncContext == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::refreshTTLAsync: m_asyncContext is nullptr";
		return;
	}

	redisAsyncCommand(this->m_asyncContext, [](redisAsyncContext* c, void* r, void* privdata)
	{
		if (r == nullptr)
		{
			BOOST_LOG_TRIVIAL(error) << "RedisContext::refreshTTLAsync: redisAsyncCommand failed";
			return;
		}
		freeReplyObject((redisReply*)r);
	}, nullptr, "EXPIRE %s %d", key.c_str(), expireSeconds);
}

std::shared_ptr<std::string> RedisContext::GetStringAsync(const std::string& key)
{
	if (this->m_asyncContext == nullptr)
	{
		BOOST_LOG_TRIVIAL(error) << "RedisContext::GetStringAsync: m_asyncContext is nullptr";
		return nullptr;
	}

	std::shared_ptr<std::string> str = std::make_shared<std::string>();

	redisAsyncCommand(this->m_asyncContext, [](redisAsyncContext* c, void* r, void* privdata)
	{
		if (r == nullptr)
		{
			BOOST_LOG_TRIVIAL(error) << "RedisContext::GetStringAsync: redisAsyncCommand failed";
			return;
		}

		redisReply* reply = (redisReply*)r;
		if (reply->type == REDIS_REPLY_NIL)
		{
			return;
		}
		if (reply->str == nullptr)
		{
			return;
		}
		auto str = (std::string*)privdata;
		*str = reply->str;
		freeReplyObject(reply);
	}, str.get(), "GET %s", key.c_str());

	if (str->empty())
	{
		return nullptr;
	}

	return str;
}

bool RedisContext::CreateAsyncContext()
{
	if (this->m_asyncContext != nullptr)
	{
		return true;
	}

	std::string m_password = ConfigProvider::GetInstance()->GetRedisPassword();

	redisOptions opt = { 0 };
	auto m_host = ConfigProvider::GetInstance()->GetRedisHost();
	(&opt)->type = REDIS_CONN_TCP;
	(&opt)->endpoint.tcp.ip = m_host.c_str();
	(&opt)->endpoint.tcp.port = 6379;
	opt.options |= REDIS_OPT_PREFER_IP_UNSPEC | REDIS_OPT_REUSEADDR | REDIS_OPT_NONBLOCK;

	auto c = redisAsyncConnectWithOptions(&opt);
	if (c == nullptr || c->err)
	{
		if (c)
		{
			BOOST_LOG_TRIVIAL(error) << "Error: " << c->errstr;
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "Can't allocate redis context";
		}
		return false;
	}
	else
	{
		this->m_asyncContext = c;
		this->m_asyncContext->data = this;

		redisAsyncSetConnectCallback(this->m_asyncContext, [](const struct redisAsyncContext* c, int status)
		{
			if (status != REDIS_OK)
			{
				BOOST_LOG_TRIVIAL(error) << "Error: " << c->errstr;
				return;
			}
			BOOST_LOG_TRIVIAL(info) << "Connected...";
		});

		redisAsyncSetDisconnectCallback(this->m_asyncContext, [](const struct redisAsyncContext* c, int status)
		{
			if (status != REDIS_OK)
			{
				BOOST_LOG_TRIVIAL(error) << "Error: " << c->errstr;
				return;
			}
			BOOST_LOG_TRIVIAL(info) << "Disconnected...";
		});
	}
	redisAsyncCommand(this->m_asyncContext, nullptr, nullptr, "AUTH %s", m_password.c_str());
	
	return true;
}
