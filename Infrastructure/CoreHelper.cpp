#include "pch.h"
#include "CoreHelper.h"
#include <xlocale>

CoreHelper::CoreHelper()
{
}

CoreHelper::~CoreHelper()
{
}

std::string CoreHelper::GetGuid()
{

	UUID uuid;
	std::string guid;
	if (UuidCreate(&uuid) == RPC_S_OK)
	{
		RPC_CSTR szUuid = NULL;
		if (UuidToStringA(&uuid, &szUuid) == RPC_S_OK)
		{
			guid = (char*)szUuid;
			RpcStringFreeA(&szUuid);
		}
	}
	return guid;
}

inline tm CoreHelper::GetSystemTime()
{
	time_t rawtime;
	time(&rawtime);
	tm tm{};
	localtime_s(&tm, &rawtime);
	return tm;
}
