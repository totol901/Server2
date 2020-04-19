#pragma once

class SystemReport : public Work
{
public:
	SystemReport() {};
	~SystemReport() {};

	void tick()
	{
		Monitoring& moniter = Monitoring::getInstance();
		SLog(L"### cpu usage : %2.2f%%, memory usage : %uByte", moniter.processCpuUsage(), moniter.processMemUsage());
	}
};
