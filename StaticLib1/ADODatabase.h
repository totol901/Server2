#pragma once

typedef ADODB::_ConnectionPtr           dbConnectionPtr;
typedef ADODB::_CommandPtr				commandPtr;
typedef ADODB::_RecordsetPtr			recordPtr;

class ADODatabase : public Database
{
	dbConnectionPtr   			dbConnection_;
	wstr_t			            connectionStr_;
	wstr_t			            dbName_;

	Thread						*thread_;

public:
	ADODatabase();
	virtual ~ADODatabase();

	HRESULT	setConnectTimeOut(long second);
	void comError(const WCHAR *actionName, _com_error &e);

	bool connect(const WCHAR *provider, const WCHAR *serverName, const WCHAR *dbName, const WCHAR *id, const WCHAR *password);
	bool connect(const WCHAR *serverName, const WCHAR *dbName, const WCHAR *id, const WCHAR *password);
	bool connect();
	bool connected();
	bool disconnect();

	void run();

private:
	void execute();
	void process();
};
