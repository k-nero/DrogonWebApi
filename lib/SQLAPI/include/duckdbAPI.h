//////////////////////////////////////////////////////////////////////
// duckdbAPI.h
//////////////////////////////////////////////////////////////////////

#if !defined(__DUCKDB_API_H__)
#define __DUCKDB_API_H__

#include <SQLAPI.h>
#include <samisc.h>

#include <duckdb.h>

typedef duckdb_state (*duckdb_open_t)(const char* path, duckdb_database* out_database);

typedef duckdb_state (*duckdb_open_ext_t)(const char* path, duckdb_database* out_database, duckdb_config config, char** out_error);

typedef void (*duckdb_close_t)(duckdb_database* database);
typedef duckdb_state (*duckdb_connect_t)(duckdb_database database, duckdb_connection* out_connection);
typedef void (*duckdb_disconnect_t)(duckdb_connection* connection);

typedef const char* (*duckdb_library_version_t)();

typedef duckdb_state (*duckdb_create_config_t)(duckdb_config* out_config);
typedef size_t (*duckdb_config_count_t)();
typedef duckdb_state (*duckdb_get_config_flag_t)(size_t index, const char** out_name, const char** out_description);
typedef duckdb_state (*duckdb_set_config_t)(duckdb_config config, const char* name, const char* option);
typedef void (*duckdb_destroy_config_t)(duckdb_config* config);

typedef duckdb_state (*duckdb_query_t)(duckdb_connection connection, const char* query, duckdb_result* out_result);
typedef void (*duckdb_destroy_result_t)(duckdb_result* result);
typedef const char* (*duckdb_column_name_t)(duckdb_result* result, idx_t col);

typedef duckdb_type (*duckdb_column_type_t)(duckdb_result* result, idx_t col);
typedef duckdb_logical_type (*duckdb_column_logical_type_t)(duckdb_result* result, idx_t col);
typedef idx_t (*duckdb_column_count_t)(duckdb_result* result);
typedef idx_t (*duckdb_row_count_t)(duckdb_result* result);
typedef idx_t (*duckdb_rows_changed_t)(duckdb_result* result);

typedef const char* (*duckdb_result_error_t)(duckdb_result* result);

typedef bool (*duckdb_value_boolean_t)(duckdb_result* result, idx_t col, idx_t row);
typedef int8_t (*duckdb_value_int8_t)(duckdb_result* result, idx_t col, idx_t row);
typedef int16_t (*duckdb_value_int16_t)(duckdb_result* result, idx_t col, idx_t row);
typedef int32_t (*duckdb_value_int32_t)(duckdb_result* result, idx_t col, idx_t row);
typedef int64_t (*duckdb_value_int64_t)(duckdb_result* result, idx_t col, idx_t row);
typedef uint8_t(*duckdb_value_uint8_t)(duckdb_result* result, idx_t col, idx_t row);
typedef uint16_t(*duckdb_value_uint16_t)(duckdb_result* result, idx_t col, idx_t row);
typedef uint32_t(*duckdb_value_uint32_t)(duckdb_result* result, idx_t col, idx_t row);
typedef uint64_t(*duckdb_value_uint64_t)(duckdb_result* result, idx_t col, idx_t row);
typedef float (*duckdb_value_float_t)(duckdb_result* result, idx_t col, idx_t row);
typedef double (*duckdb_value_double_t)(duckdb_result* result, idx_t col, idx_t row);
typedef duckdb_blob (*duckdb_value_blob_t)(duckdb_result* result, idx_t col, idx_t row);

typedef duckdb_hugeint (*duckdb_value_hugeint_t)(duckdb_result* result, idx_t col, idx_t row);
typedef duckdb_decimal (*duckdb_value_decimal_t)(duckdb_result* result, idx_t col, idx_t row);
typedef duckdb_date (*duckdb_value_date_t)(duckdb_result* result, idx_t col, idx_t row);
typedef duckdb_time (*duckdb_value_time_t)(duckdb_result* result, idx_t col, idx_t row);
typedef duckdb_timestamp (*duckdb_value_timestamp_t)(duckdb_result* result, idx_t col, idx_t row);
typedef duckdb_interval(*duckdb_value_interval_t)(duckdb_result* result, idx_t col, idx_t row);
typedef duckdb_string (*duckdb_value_string_t)(duckdb_result* result, idx_t col, idx_t row);
typedef bool (*duckdb_value_is_null_t)(duckdb_result* result, idx_t col, idx_t row);

typedef duckdb_state (*duckdb_prepare_t)(duckdb_connection connection, const char* query,
	duckdb_prepared_statement* out_prepared_statement);
typedef duckdb_state (*duckdb_nparams_t)(duckdb_prepared_statement prepared_statement, idx_t* nparams_out);
typedef duckdb_state (*duckdb_bind_boolean_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, bool val);
typedef duckdb_state (*duckdb_bind_int8_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, int8_t val);
typedef duckdb_state (*duckdb_bind_int16_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, int16_t val);
typedef duckdb_state (*duckdb_bind_int32_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, int32_t val);
typedef duckdb_state (*duckdb_bind_int64_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, int64_t val);
typedef duckdb_state (*duckdb_bind_float_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, float val);
typedef duckdb_state (*duckdb_bind_double_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, double val);
typedef duckdb_state (*duckdb_bind_varchar_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, const char* val);
typedef duckdb_state (*duckdb_bind_null_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx);
typedef duckdb_state (*duckdb_bind_blob_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, const void* data, idx_t length);

typedef duckdb_state (*duckdb_bind_uint16_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, uint16_t val);
typedef duckdb_state (*duckdb_bind_uint32_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, uint32_t val);
typedef duckdb_state (*duckdb_bind_uint64_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, uint64_t val);
typedef duckdb_state (*duckdb_bind_date_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, duckdb_date val);
typedef duckdb_state (*duckdb_bind_time_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, duckdb_time val);
typedef duckdb_state (*duckdb_bind_timestamp_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, duckdb_timestamp val);
typedef duckdb_state (*duckdb_bind_interval_t)(duckdb_prepared_statement prepared_statement, idx_t param_idx, duckdb_interval val);

typedef duckdb_state (*duckdb_execute_prepared_t)(duckdb_prepared_statement prepared_statement, duckdb_result* out_result);
typedef void (*duckdb_destroy_prepare_t)(duckdb_prepared_statement* prepared_statement);

typedef idx_t (*duckdb_column_count_t)(duckdb_result* result);

typedef void (*duckdb_free_t)(void* ptr);

typedef duckdb_date_struct (*duckdb_from_date_t)(duckdb_date date);
typedef duckdb_date (*duckdb_to_date_t)(duckdb_date_struct date);
typedef duckdb_time_struct (*duckdb_from_time_t)(duckdb_time time);
typedef duckdb_time (*duckdb_to_time_t)(duckdb_time_struct time);
typedef duckdb_timestamp_struct (*duckdb_from_timestamp_t)(duckdb_timestamp ts);
typedef duckdb_timestamp (*duckdb_to_timestamp_t)(duckdb_timestamp_struct ts);

// API declarations
class SQLAPI_API duckdbAPI : public IsaAPI
{
public:
	duckdbAPI();

public:
	duckdb_open_t duckdb_open;
	duckdb_close_t duckdb_close;
	duckdb_connect_t duckdb_connect;
	duckdb_disconnect_t duckdb_disconnect;
	duckdb_query_t duckdb_query;
	duckdb_destroy_result_t duckdb_destroy_result;
	duckdb_column_name_t duckdb_column_name;
	duckdb_value_boolean_t duckdb_value_boolean;
	duckdb_value_int8_t duckdb_value_int8;
	duckdb_value_int16_t duckdb_value_int16;
	duckdb_value_int32_t duckdb_value_int32;
	duckdb_value_int64_t duckdb_value_int64;
	duckdb_value_uint8_t duckdb_value_uint8;
	duckdb_value_uint16_t duckdb_value_uint16;
	duckdb_value_uint32_t duckdb_value_uint32;
	duckdb_value_uint64_t duckdb_value_uint64;
	duckdb_value_float_t duckdb_value_float;
	duckdb_value_double_t duckdb_value_double;
	duckdb_value_blob_t duckdb_value_blob;
	duckdb_prepare_t duckdb_prepare;
	duckdb_nparams_t duckdb_nparams;
	duckdb_bind_boolean_t duckdb_bind_boolean;
	duckdb_bind_int8_t duckdb_bind_int8;
	duckdb_bind_int16_t duckdb_bind_int16;
	duckdb_bind_int32_t duckdb_bind_int32;
	duckdb_bind_int64_t duckdb_bind_int64;
	duckdb_bind_float_t duckdb_bind_float;
	duckdb_bind_double_t duckdb_bind_double;
	duckdb_bind_varchar_t duckdb_bind_varchar;
	duckdb_bind_null_t duckdb_bind_null;
	duckdb_bind_blob_t duckdb_bind_blob;
	duckdb_execute_prepared_t duckdb_execute_prepared;
	duckdb_destroy_prepare_t duckdb_destroy_prepare;

	duckdb_column_count_t duckdb_column_count;
	duckdb_column_type_t duckdb_column_type;
	duckdb_row_count_t duckdb_row_count;
	duckdb_rows_changed_t duckdb_rows_changed;
	duckdb_library_version_t duckdb_library_version;
	duckdb_value_is_null_t duckdb_value_is_null;
	duckdb_free_t duckdb_free;
	duckdb_value_string_t duckdb_value_string;
	duckdb_value_interval_t duckdb_value_interval;
	duckdb_value_date_t duckdb_value_date;
	duckdb_value_time_t duckdb_value_time;
	duckdb_value_timestamp_t duckdb_value_timestamp;
	duckdb_from_date_t duckdb_from_date;
	duckdb_to_date_t duckdb_to_date;
	duckdb_from_time_t duckdb_from_time;
	duckdb_to_time_t duckdb_to_time;
	duckdb_from_timestamp_t duckdb_from_timestamp;
	duckdb_to_timestamp_t duckdb_to_timestamp;
	duckdb_result_error_t duckdb_result_error;
	duckdb_bind_uint16_t duckdb_bind_uint16;
	duckdb_bind_uint32_t duckdb_bind_uint32;
	duckdb_bind_uint64_t duckdb_bind_uint64;
	duckdb_bind_date_t duckdb_bind_date;
	duckdb_bind_time_t duckdb_bind_time;
	duckdb_bind_timestamp_t duckdb_bind_timestamp;
	duckdb_bind_interval_t duckdb_bind_interval;

public:
	virtual void InitializeClient(const SAOptions *pOptions);
	virtual void UnInitializeClient(const SAPI *pSAPI, const SAOptions* pOptions);

	virtual long GetClientVersion() const;

	virtual ISAConnection *NewConnection(SAConnection *pConnection);

protected:
	void  *m_hLibrary;
	SAMutex m_loaderMutex;

	void ResetAPI();
	void LoadAPI();
};

class SQLAPI_API duckdbConnectionHandles : public saConnectionHandles
{
public:
	duckdbConnectionHandles();
	duckdb_database db;
	duckdb_connection conn;
};

class SQLAPI_API duckdbCommandHandles : public saCommandHandles
{
public:
	duckdbCommandHandles();
	
	duckdb_result result;
	duckdb_prepared_statement stmt;

	void reset();
};

#endif //__DUCKDB_API_H__
