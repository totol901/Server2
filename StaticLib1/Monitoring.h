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
	함수명	: processCpuUsage
	설명		: 프로세스가 사용중인 CPU %
	리턴값	: double
	매개변수	: 없음
	*****************************************************************************/
	double processCpuUsage()
	{
		FILETIME ftime, fsys, fuser;
		ULARGE_INTEGER now, sys, user;
		double percent;

		GetSystemTimeAsFileTime(&ftime);//현재 시간
		memcpy(&now, &ftime, sizeof(FILETIME));

		GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
		memcpy(&sys, &fsys, sizeof(FILETIME));	//커널모드에서 사용 시간
		memcpy(&user, &fuser, sizeof(FILETIME));//유저모드에서 사용 시간
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
	함수명	: processMemUsage
	설명		: 현재 프로세스가 쓰고있는 메모리
	리턴값	: SIZE_T
	매개변수	: 없음
	*****************************************************************************/
	SIZE_T processMemUsage()
	{
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
		return (size_t)pmc.WorkingSetSize;
	}

	/****************************************************************************
	함수명	: physyicMemUsage
	설명		: 지금 쓰고 있는 전체 메모리
	리턴값	: SIZE_T
	매개변수	: 없음
	*****************************************************************************/
	SIZE_T physyicMemUsage()
	{
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);

		return (size_t)memInfo.ullTotalPhys - memInfo.ullAvailPhys;
	}
};
