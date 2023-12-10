#pragma once
#include <windows.h>
#include <string>
#include <rpc.h>
#include <rpcdce.h>

#ifdef INFRASTRUCTURE_EXPORTS
#define INFRASTRUCTURE_API __declspec(dllexport)
#else
#define INFRASTRUCTURE_API __declspec(dllimport)
#endif

class INFRASTRUCTURE_API CoreHelper
{
public:
	CoreHelper();
	~CoreHelper();
	static inline std::string GetGuid();
	static inline tm GetSystemTime();
};