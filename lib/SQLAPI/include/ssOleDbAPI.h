// ssOleDbAPI.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(__SSOLEDBAPI_H__)
#define __SSOLEDBAPI_H__

#include "SQLAPI.h"

// API header(s)
#include <oledb.h>

// API declarations
class ssOleDbAPI : public IsaAPI
{
public:
	ssOleDbAPI();

public:
	virtual void InitializeClient(const SAOptions *pOptions);
	virtual void UnInitializeClient(const SAPI* pSAPI, const SAOptions* pOptions);

	virtual long GetClientVersion() const;

	virtual ISAConnection *NewConnection(SAConnection *pConnection);

protected:
	bool m_bProcessSQLServerErrorInfo;

	void ResetAPI();

public:
	bool& ProcessSQLServerErrorInfo();

	static void CheckAndFreePropertySets(ULONG cPropertySets, DBPROPSET *rgPropertySets);
	static void CheckHRESULT(HRESULT hr);
	void Check(HRESULT hrOLEDB, IUnknown * pIUnknown, REFIID riid) const;
	void Check(const SAString &sCommandText, HRESULT hrOLEDB, IUnknown * pIUnknown, REFIID riid) const;
};

class SQLAPI_API ssOleDbConnectionHandles : public saConnectionHandles
{
public:
	ssOleDbConnectionHandles();

	IDBInitialize *pIDBInitialize;
	IDBDataSourceAdmin *pIDBDataSourceAdmin;

	IDBCreateCommand *pIDBCreateCommand;
	ITransactionLocal *pITransactionLocal;

	bool compactEdition;
};

class SQLAPI_API ssOleDbCommandHandles : public saCommandHandles
{
public:
	ssOleDbCommandHandles();

	ICommandText *pICommandText;
	IMultipleResults *pIMultipleResults;
	IRowset *pIRowset;
};

extern const GUID SA_DBPROPSET_DATASOURCEINFO;

#define SACON_OPTION_OLEDB_PROVIDER _TSA("OLEDBProvider")
#define SACON_OPTION_OLEDB_INIT _TSA("CoInitializeEx_COINIT")

#define SACMD_OPTION_OLEDB_COMMAND_PREPARE _TSA("ICommandPrepare")
#define SACMD_OPTION_OLEDB_COMMAND_EXECUTE_RIID _TSA("Execute_riid")
#define SAPAR_OPTION_OLEDB_USE_STREAM _TSA("UseStreamForLongOrLobParameters")

#endif // !defined(__SSOLEDBAPI_H__)
