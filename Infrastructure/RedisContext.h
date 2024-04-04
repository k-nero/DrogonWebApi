#pragma once

#include "ConfigProvider.h"
#include "hiredis/hiredis.h"
#include "hiredis/async.h"
#include <boost/log/trivial.hpp>
#include <mutex>
#include <shared_mutex>
#include <WinSock2.h>

class INFRASTRUCTURE_API RedisContext
{
public:
	bool CreateSyncContext(redisOptions opt);
	bool CreateSyncContext();
	void SelectDb(int dbIndex);
	void SetString(const std::string& key, const std::string& value, int expireSeconds);
	void refreshTTL(const std::string& key, int expireSeconds);
	void RemoveKey(const std::string& key);
	long long GetNumOfDb();
	std::shared_ptr<std::string> GetString(const std::string& key);
	std::vector<std::string> GetAllActiveKeys(std::string contain = "");

	int RemoveKeyContaining(const std::string& key_word);

	void SetStringAsync(const std::string& key, const std::string& value, int expireSeconds);
	void refreshTTLAsync(const std::string& key, int expireSeconds);
	std::shared_ptr<std::string> GetStringAsync(const std::string& key);
	bool CreateAsyncContext();

	static RedisContext& GetInstance()
	{
		static RedisContext instance;
		return instance;
	}

private:
	RedisContext() = default;
	~RedisContext();
	redisContext* m_syncContext = nullptr;
	redisAsyncContext* m_asyncContext = nullptr;
	mutable std::shared_mutex mutex_;
};
