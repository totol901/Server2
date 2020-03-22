#include "pch.h"
#include "DBAgentClass.h"

DBAgentClass::DBAgentClass()
{
}

DBAgentClass::~DBAgentClass()
{
}

HRESULT DBAgentClass::init()
{
	xml_t config;
	if (!loadConfig(&config))
	{
		return E_FAIL;
	}

#if _DEBUG
	HRESULT hr = WINSOCK.init();
	if (FAILED(hr))
	{
		SLog(L"! WINSOCK Init Failed");
	}

	hr = CLOCK.init();
	if (FAILED(hr))
	{
		SLog(L"! CLOCK Init Failed");
	}

	hr = SESSIONMANAGER.init();
	if (FAILED(hr))
	{
		SLog(L"! THREADMANAGER Init Failed");
	}

	hr = LOCKAMANGER.init();
	if (FAILED(hr))
	{
		SLog(L"! LOCKAMANGER Init Failed");
	}

	hr = THREADMANAGER.init();
	if (FAILED(hr))
	{
		SLog(L"! THREADMANAGER Init Failed");
	}

	TASKMANAGER.initialize(&config);

	SLog(L"### Singleton set ###");

	return hr;
#else
	WINSOCK.init();
	CLOCK.init();
	SESSIONMANAGER.init();
	LOCKAMANGER.init();
	THREADMANAGER.init();
	TASKMANAGER.initialize(&config);

	return S_OK;
#endif
}

void DBAgentClass::release()
{
	TASKMANAGER.release();
	THREADMANAGER.release();
	LOCKAMANGER.release();
	SESSIONMANAGER.release();
	CLOCK.release();
	WINSOCK.release();

	SLog(L"### Singleton release ###");
}

void DBAgentClass::serverProcess()
{
	std::shared_ptr<Server> server(new IOCPServer(new DBAgentProcess()));

	SystemReport* systemReport = new SystemReport();

	const int MONITOR_REPORTING_SEC = 1;
	TASKMANAGER.add(systemReport, MONITOR_REPORTING_SEC, TICK_INFINTY);

	if (!server->run())
	{
		SLog(L"! error: server start fail");
		return;
	}
}

void DBAgentClass::run()
{
	std::shared_ptr<Thread> serverThread(new Thread(new thread_t(&DBAgentClass::serverProcess, this), L"DBAgent"));
}
