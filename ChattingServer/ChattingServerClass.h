#pragma once

class SystemReport : public Work
{
	void tick()
	{
		Monitoring &moniter = Monitoring::getInstance();
		SLog(L"### cpu usage : %2.2f%%, memory usage : %uByte", moniter.processCpuUsage(), moniter.processMemUsage());
	}
};

class ChattingServerClass
{
private:
	void serverProcess();

public:
	ChattingServerClass();
	~ChattingServerClass();

	HRESULT init();
	void release();

	void run();
};

