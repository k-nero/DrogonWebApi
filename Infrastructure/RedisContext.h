#pragma once

#include "ConfigProvider.h"
#include "hiredis/hiredis.h"
#include "hiredis/async.h"
#include <boost/log/trivial.hpp>

class INFRASTRUCTURE_API RedisContext
{
public:
	RedisContext() = default;
	~RedisContext();

	bool CreateSyncContext(redisOptions opt);
	bool CreateSyncContext();
	void SelectDb(int dbIndex);
	void SetString(const std::string& key, const std::string& value, int expireSeconds);
	void refreshTTL(const std::string& key, int expireSeconds);
	void RemoveKey(const std::string& key);
	int GetNumOfDb();
	std::shared_ptr<std::string> GetString(const std::string& key);
	std::vector<std::string> GetAllActiveKeys(std::string contain = "");
	void CreateAsyncContext();

private:
	redisContext* m_syncContext = nullptr;
	redisAsyncContext* m_asyncContext = nullptr;
};

