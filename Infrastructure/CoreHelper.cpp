#include "pch.h"
#include "CoreHelper.h"

CoreHelper::CoreHelper()
{
}

CoreHelper::~CoreHelper()
{
}

[[nodiscard]]
inline std::string CoreHelper::CreateUUID()
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

[[nodiscard]]
inline tm CoreHelper::GetSystemTime()
{
	time_t rawtime;
	time(&rawtime);
	tm tm{};
	localtime_s(&tm, &rawtime);
	return tm;
}

[[nodiscard]]
inline std::string CoreHelper::GetSystemTimeAsString(tm tm)
{
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
	return std::string(buffer);
}

tm CoreHelper::GetSystemTimeFromString(std::string time)
{
	std::tm t{};

	if (std::sscanf(time.data(), "%d-%d-%d %d %d %d",
		&t.tm_year,
		&t.tm_mon,
		&t.tm_mday,
		&t.tm_hour,
		&t.tm_min,
		&t.tm_sec
	) != 6)
	{
		throw std::runtime_error("Invalid date format: " + std::string(time));

	}

	t.tm_year -= 1900;
	--t.tm_mon;
	t.tm_isdst = -1;  // guess if DST should be in effect when calling mktime
	errno = 0;
	std::mktime(&t);

	return t;
}
[[nodiscard]]
inline std::string CoreHelper::ReadTextFile(std::string path) noexcept(false)
{
	try
	{
		std::ifstream pemFile(path);
		if (!pemFile.is_open())
		{
			BOOST_LOG_TRIVIAL(error) << "Error reading file: " << path;
			return "";
		}
		SkipBOM(pemFile);
		std::stringstream strStream;
		strStream << pemFile.rdbuf(); // read the file
		std::string pemToString = strStream.str(); // str holds the content of the file
		return pemToString;
	}
	catch (const std::exception&)
	{
		BOOST_LOG_TRIVIAL(error) << "Error reading file: " << path;
		return "";
	}
}

[[nodiscard]]
inline Json::Value CoreHelper::ParseJson(std::string& json)
{
	Json::Value root;
	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();
	std::string errors;
	bool parsingSuccessful = reader->parse(json.c_str(), json.c_str() + json.size(), &root, &errors);
	delete reader;
	if (!parsingSuccessful)
	{
		root = Json::Value();
		root["error"] = errors;
	}
	return root;
}

inline void CoreHelper::SkipBOM(std::ifstream& in)
{
	char test[3] = { 0 };
	in.read(test, 3);
	if ((unsigned char)test[0] == 0xEF &&
		(unsigned char)test[1] == 0xBB &&
		(unsigned char)test[2] == 0xBF)
	{
		return;
	}
	in.seekg(0);
}


