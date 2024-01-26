#pragma once
#include <windows.h>
#include <string>
#include <rpc.h>
#include <rpcdce.h>
#include <xlocale>
#include <fstream>
#include <sstream>
#include "InfrastructureApi.h"
#include <boost/log/trivial.hpp>


class INFRASTRUCTURE_API CoreHelper
{
public:
	CoreHelper();
	~CoreHelper();
	static inline std::string CreateUUID();
	static inline tm GetSystemTime();
	static inline std::string ReadTextFile(std::string path);
private:
	inline static void SkipBOM(std::ifstream& in);
};