#include "pch.h"
#include "SQLAPIClient.h"

SQLAPIClient::SQLAPIClient()
{
	if (ConfigProvider::GetInstance()->GetConnectionString().server.empty())
	{
		ConfigProvider::GetInstance()->Initialize();
	}
	auto connectionString = ConfigProvider::GetInstance()->GetConnectionString();
	try
	{
		connection->Connect(connectionString.server.c_str(), connectionString.username.c_str(), connectionString.password.c_str(), SA_SQLServer_Client);
		if (connection != nullptr)
		{
			if (!connection->isConnected())
			{
				BOOST_LOG_TRIVIAL(fatal) << "Unable to connect to the database";
				throw std::exception("Unable to connect to the database");
			}
		}
	}
	catch (SAException& x)
	{
		BOOST_LOG_TRIVIAL(fatal) << x.ErrText();
		BOOST_LOG_TRIVIAL(error) << x.ErrMessage();
#ifdef _DEBUG
		BOOST_LOG_TRIVIAL(debug) << x.ErrNativeCode();
		BOOST_LOG_TRIVIAL(debug) << x.ErrClass();
		BOOST_LOG_TRIVIAL(debug) << x.ErrPos();
#endif 
		throw std::exception(x.ErrText().GetMultiByteChars());
	}
	catch (std::exception& ex)
	{
		BOOST_LOG_TRIVIAL(fatal) << ex.what();
		throw ex;
	}
	catch (...)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Unknown exception";
	}
}

SQLAPIClient::~SQLAPIClient()
{
	if (connection->isConnected())
	{
		connection->Disconnect();
	}
	delete connection;
}

void SQLAPIClient::CreateCommand(const std::string& command_text) noexcept(false)
{
	cmd = std::make_shared<SACommand>(connection, command_text.c_str(), SA_CmdSQLStmtRaw);
}

void SQLAPIClient::ExecuteCommand() noexcept(false)
{
	cmd->Execute();
}
bool SQLAPIClient::FetchNext() noexcept(false)
{
	return cmd->FetchNext();
}

std::shared_ptr<std::string> SQLAPIClient::GetJsonStringResult(const std::string& column_name) noexcept(false)
{
	auto result = std::make_shared<std::string>();
	cmd->Field(column_name.c_str()).setFieldType(SA_dtLongChar);
	cmd->Field(column_name.c_str()).setFieldSize(0x800);
	cmd->Field(column_name.c_str()).setLongOrLobReaderMode(SALongOrLobReaderModes_t::SA_LongOrLobReaderManual);
	while (cmd->FetchNext())
	{

		auto HandlingJson = [](SAPieceType_t ePieceType, void* pBuf, size_t nLen, size_t nBlobSize, void* pAddlData) -> void
		{
			if (ePieceType == SAPieceType_t::SA_FirstPiece || ePieceType == SAPieceType_t::SA_NextPiece)
			{
				auto result = static_cast<std::string*>(pAddlData);
				result->append(static_cast<char*>(pBuf), nLen);
			}
		};

		cmd->Field(column_name.c_str()).ReadLongOrLob(HandlingJson, 0x800, (void*)(result.get()));

	}
	return result;
}

std::shared_ptr<std::string> SQLAPIClient::GetJsonStringResult(const int column_index) noexcept(false)
{
	auto result = std::make_shared<std::string>();
	cmd->Field(column_index).setFieldType(SA_dtLongChar);
	cmd->Field(column_index).setFieldSize(0x800);
	cmd->Field(column_index).setLongOrLobReaderMode(SALongOrLobReaderModes_t::SA_LongOrLobReaderManual);
	while (cmd->FetchNext())
	{

		auto HandlingJson = [](SAPieceType_t ePieceType, void* pBuf, size_t nLen, size_t nBlobSize, void* pAddlData) -> void
		{
			if (ePieceType == SAPieceType_t::SA_FirstPiece || ePieceType == SAPieceType_t::SA_NextPiece)
			{
				auto result = static_cast<std::string*>(pAddlData);
				result->append(static_cast<char*>(pBuf), nLen);
			}
		};

		cmd->Field(column_index).ReadLongOrLob(HandlingJson, 0x800, (void*)(result.get()));

	}
	return result;
}

inline void SQLAPIClient::BindParameter(const std::string& parameter_name, const std::string& value) noexcept(false)
{
	cmd->Param(parameter_name.c_str()).setAsString() = value.c_str();
}

inline void SQLAPIClient::BindParameter(const std::string& parameter_name, const int value) noexcept(false)
{
	cmd->Param(parameter_name.c_str()).setAsLong() = value;
}

inline void SQLAPIClient::BindParameter(const std::string& parameter_name, const long value) noexcept(false)
{
	cmd->Param(parameter_name.c_str()).setAsLong() = value;
}

inline void SQLAPIClient::BindParameter(const std::string& parameter_name, const bool value) noexcept(false)
{
	cmd->Param(parameter_name.c_str()).setAsBool() = value;
}

inline void SQLAPIClient::BindParameter(const std::string& parameter_name, const double value) noexcept(false)
{
	cmd->Param(parameter_name.c_str()).setAsDouble() = value;
}

inline void SQLAPIClient::BindParameter(const std::string& parameter_name, const std::tm value) noexcept(false)
{
	cmd->Param(parameter_name.c_str()).setAsDateTime() = SADateTime(value);
}

inline void SQLAPIClient::BindParameter(const std::string& parameter_name, const std::wstring value) noexcept(false)
{
	cmd->Param(parameter_name.c_str()).setAsString() = value.data();
}

inline int SQLAPIClient::GetIntResult(const std::string& column_name) noexcept(false)
{
	return cmd->Field(column_name.c_str()).asInt32();
}

inline int SQLAPIClient::GetIntResult(const int column_index) noexcept(false)
{
	return cmd->Field(column_index).asInt32();
}

inline long SQLAPIClient::GetLongResult(const std::string& column_name) noexcept(false)
{
	return cmd->Field(column_name.c_str()).asInt64();
}

inline long SQLAPIClient::GetLongResult(const int column_index) noexcept(false)
{
	return cmd->Field(column_index).asInt64();
}

inline bool SQLAPIClient::GetBoolResult(const std::string& column_name) noexcept(false)
{
	return cmd->Field(column_name.c_str()).asBool();
}

inline bool SQLAPIClient::GetBoolResult(const int column_index) noexcept(false)
{
	return cmd->Field(column_index).asBool();
}

inline double SQLAPIClient::GetDoubleResult(const std::string& column_name) noexcept(false)
{
	return cmd->Field(column_name.c_str()).asDouble();
}

inline double SQLAPIClient::GetDoubleResult(const int column_index) noexcept(false)
{
	return cmd->Field(column_index).asDouble();
}

inline std::tm SQLAPIClient::GetDateTimeResult(const std::string& column_name) noexcept(false)
{
	return cmd->Field(column_name.c_str()).asDateTime();
}

inline std::tm SQLAPIClient::GetDateTimeResult(const int column_index) noexcept(false)
{
	return cmd->Field(column_index).asDateTime();
}

inline std::string SQLAPIClient::GetStringResult(const std::string& column_name) noexcept(false)
{
	return cmd->Field(column_name.c_str()).asString().GetMultiByteChars();
}

inline std::string SQLAPIClient::GetStringResult(const int column_index) noexcept(false)
{
	return cmd->Field(column_index).asString().GetMultiByteChars();
}
