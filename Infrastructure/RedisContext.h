#pragma once

#include "ConfigProvider.h"
#include "hiredis/hiredis.h"
#include "hiredis/async.h"
#include <boost/log/trivial.hpp>

class RedisContext
{
public:
	RedisContext() = default;
	~RedisContext();

	bool CreateSyncContext(redisOptions opt);
	bool CreateSyncContext();
	void CreateAsyncContext();

private:
	redisContext* m_syncContext = nullptr;
	redisAsyncContext* m_asyncContext = nullptr;
};

