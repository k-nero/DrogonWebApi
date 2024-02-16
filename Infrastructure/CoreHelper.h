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
	static std::string CreateUUID();
	static tm GetSystemTime();
	static std::string ReadTextFile(std::string path);
private:
	static void SkipBOM(std::ifstream& in);
};