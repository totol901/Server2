#pragma once

class SystemReport : public Work
{
public:
	SystemReport() {};
	~SystemReport() {};
	
	void tick()
	{
		Monitoring &moniter = Monitoring::getInstance();
		SLog(L"### cpu usage : %2.2f%%, memory usage : %uByte", moniter.processCpuUsage(), moniter.processMemUsage());
	}
};

class LoginServerClass
{
private:
	void serverProcess();

public:
	LoginServerClass();
	~LoginServerClass();

	HRESULT init();
	void release();

	void run();
};
