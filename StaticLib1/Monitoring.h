#pragma once

class Monitoring : public Singleton<Monitoring>
{
	friend Singleton;
private:
	ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	int numProcessors;
	HANDLE self;

	Monitoring()
	{
		SYSTEM_INFO sysInfo;
		FILETIME ftime, fsys, fuser;

		GetSystemInfo(&sysInfo);
		numProcessors = sysInfo.dwNumberOfProcessors;

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&lastCPU, &ftime, sizeof(FILETIME));

		self = GetCurrentProcess();
		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
		memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
	}
	~Monitoring() {}

public:
	/****************************************************************************
	�Լ���	: processCpuUsage
	����		: ���μ����� ������� CPU %
	���ϰ�	: double
	�Ű�����	: ����
	*****************************************************************************/
	double processCpuUsage()
	{
		FILETIME ftime, fsys, fuser;
		ULARGE_INTEGER now, sys, user;
		double percent;

		GetSystemTimeAsFileTime(&ftime);//���� �ð�
		memcpy(&now, &ftime, sizeof(FILETIME));

		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&sys, &fsys, sizeof(FILETIME));	//Ŀ�θ�忡�� ��� �ð�
		memcpy(&user, &fuser, sizeof(FILETIME));//������忡�� ��� �ð�
		percent = (double)((sys.QuadPart - lastSysCPU.QuadPart) + (user.QuadPart - lastUserCPU.QuadPart));
		percent /= (now.QuadPart - lastCPU.QuadPart);
		percent /= numProcessors;
		//lastCPU = now;
		//lastUserCPU = user;
		//lastSysCPU = sys;
		percent = percent * 100;
		return fixInRange(0, percent, 100);
	}

	/****************************************************************************
	�Լ���	: processMemUsage
	����		: ���� ���μ����� �����ִ� �޸�
	���ϰ�	: SIZE_T
	�Ű�����	: ����
	*****************************************************************************/
	SIZE_T processMemUsage()
	{
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
		return (size_t)pmc.WorkingSetSize;
	}

	/****************************************************************************
	�Լ���	: physyicMemUsage
	����		: ���� ���� �ִ� ��ü �޸�
	���ϰ�	: SIZE_T
	�Ű�����	: ����
	*****************************************************************************/
	SIZE_T physyicMemUsage()
	{
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);

		return (size_t)memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	}
};
