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
#include <json/json.h>
#include <stdio.h> 
#include <stdlib.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>

class INFRASTRUCTURE_API CoreHelper
{
public:
	CoreHelper();
	~CoreHelper();
	static std::string CreateUUID();
	static tm GetSystemTime();
	static std::string GetSystemTimeAsString(tm tm);
	static tm GetSystemTimeFromString(std::string time);
	static std::string ReadTextFile(std::string path);
	static Json::Value ParseJson(std::string& json);
	static std::vector<std::string> GetAllListenAddresses(std::string init_ip);
	
private:
	static void SkipBOM(std::ifstream& in);
};

