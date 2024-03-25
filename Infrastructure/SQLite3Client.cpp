#include "pch.h"
#include "SQLite3Client.h"

SQLite3Client::SQLite3Client()
{
	db = std::make_shared<SQLite::Database>(":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
}

SQLite3Client::~SQLite3Client()
{
}

void SQLite3Client::CreateCommand(const std::string& command_text) noexcept(false)
{
	stmt = std::make_shared<SQLite::Statement>(*db, command_text);
}

void SQLite3Client::ExecuteCommand() noexcept(false)
{
	stmt->exec();
}

bool SQLite3Client::FetchNext() noexcept(false)
{
	return stmt->executeStep();
}

void SQLite3Client::GetJsonStringResult(std::shared_ptr<std::string>& result) noexcept(false)
{
	
}


inline void SQLite3Client::BindParameter(const std::string& parameter_name, const std::string& value) noexcept(false)
{
	this->stmt->bind(parameter_name, value);
}

inline void SQLite3Client::BindParameter(const std::string& parameter_name, const int value) noexcept(false)
{
	this->stmt->bind(parameter_name, value);
}

void SQLite3Client::BindParameter(const std::string& parameter_name, const long long value) noexcept(false)
{
	this->stmt->bind(parameter_name, value);
}

inline void SQLite3Client::BindParameter(const std::string& parameter_name, const long value) noexcept(false)
{
	this->stmt->bind(parameter_name, (long long)value);
}

inline void SQLite3Client::BindParameter(const std::string& parameter_name, const bool value) noexcept(false)
{
	this->stmt->bind(parameter_name, value);
}

inline void SQLite3Client::BindParameter(const std::string& parameter_name, const double value) noexcept(false)
{
	this->stmt->bind(parameter_name, value);
}

inline void SQLite3Client::BindParameter(const std::string& parameter_name, const std::tm value) noexcept(false)
{
	std::string date = CoreHelper::GetSystemTimeAsString(value);
	this->stmt->bind(parameter_name, date);
}

inline void SQLite3Client::BindParameter(const std::string& parameter_name, const std::wstring value) noexcept(false)
{
	this->stmt->bind(parameter_name, (char*)value.c_str());
}

inline int SQLite3Client::GetIntResult(const std::string& column_name) noexcept(false)
{
	return stmt->getColumn(column_name.c_str()).getInt();
}

inline int SQLite3Client::GetIntResult(const int column_index) noexcept(false)
{
	return stmt->getColumn(column_index).getInt();
}

inline long long SQLite3Client::GetLongResult(const std::string& column_name) noexcept(false)
{
	return stmt->getColumn(column_name.c_str()).getInt64();
}

inline long long SQLite3Client::GetLongResult(const int column_index) noexcept(false)
{
	return stmt->getColumn(column_index).getInt64();
}

inline bool SQLite3Client::GetBoolResult(const std::string& column_name) noexcept(false)
{
	return stmt->getColumn(column_name.c_str()).getInt();
}

inline bool SQLite3Client::GetBoolResult(const int column_index) noexcept(false)
{
	return stmt->getColumn(column_index).getInt();
}

inline double SQLite3Client::GetDoubleResult(const std::string& column_name) noexcept(false)
{
	return stmt->getColumn(column_name.c_str()).getDouble();
}

inline double SQLite3Client::GetDoubleResult(const int column_index) noexcept(false)
{
	return stmt->getColumn(column_index).getDouble();	
}

inline std::tm SQLite3Client::GetDateTimeResult(const std::string& column_name) noexcept(false)
{
	return CoreHelper::GetSystemTimeFromString(stmt->getColumn(column_name.c_str()).getText());
}


inline std::tm SQLite3Client::GetDateTimeResult(const int column_index) noexcept(false)
{
	return CoreHelper::GetSystemTimeFromString(stmt->getColumn(column_index).getText());
}

inline std::string SQLite3Client::GetStringResult(const std::string& column_name) noexcept(false)
{
	return stmt->getColumn(column_name.c_str()).getText();
}

inline std::string SQLite3Client::GetStringResult(const int column_index) noexcept(false)
{
	return stmt->getColumn(column_index).getText();
}

long long SQLite3Client::AffectedRows() noexcept(false)
{
	return stmt->getChanges();
}

void SQLite3Client::TestClient() noexcept(false)
{
	BOOST_LOG_TRIVIAL(debug) << "SQLite3Client::TestClient()";
}
