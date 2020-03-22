// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.

///라이브러리 사용!
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <mstcpip.h>
#include <Ws2spi.h>
#include <Mstcpip.h>
#include <mswsock.h>
#include <Mmsystem.h>
#include <tchar.h>

#include <Windows.h>
#include <iostream>
#include <functional>
#include <thread>
#include <ctime>
#include <mutex>
#include <map>
#include <vector>
#include <unordered_map>
#include <list>
#include <array>
#include <chrono>
#include <ctime>
#include <string>
#include <queue>
#include <fstream>
#include <psapi.h>
#include <unordered_map>

#import "c:\Program Files\Common Files\System\ADO\msado15.dll"  rename("EOF", "EndOfFile")

#pragma comment(lib, "ws2_32.lib")

#include "tinyxml.h"

#include "type.h"
#include "Util.h"
#include "Singleton.h"
#include "Config.h"
#include "Shutdown.h"
#include "GameObject.h"

#include "WinSocket.h"
#include "Logger.h"
#include "Monitoring.h"
#include "Clock.h"

#include "Lock.h"
#include "Thread.h"
#include "ThreadJobQueue.h"

#include "Task.h"

#include "Stream.h"

#include "PacketHeader.h"
#include "PacketClass.h"
#include "PacketFactory.h"

#include "Session.h"
#include "SessionManager.h"
#include "IOCPSession.h"

#include "Package.h"

#include "PacketAnalyzer.h"

#include "ContentsProcess.h"

#include "Server.h"
#include "IOCPServer.h"

#include "TerminalSession.h"
#include "Terminal.h"
#include "TerminalManager.h"

#include "Database.h"
#include "ADODatabase.h"
#include "QueryRecord.h"
#include "QueryStatement.h"
#include "Query.h"
#include "DBManager.h"

#if _DEBUG
#define CONTEXT_SWITCH		Sleep(1)
#else
#define CONTEXT_SWITCH		::SwitchToThread()
#endif

typedef void(*Function)(void *);

#define WINSOCK WinSocket::getInstance()
#define TERMINALMANAGER TerminalManager::getInstance()
#define LOCKAMANGER LockManager::getInstance()
#define THREADMANAGER ThreadManager::getInstance()
#define SESSIONMANAGER SessionManager::getInstance()
#define CLOCK Clock::getInstance()
#define PACKETFACTORY PacketFactory::getInstance()
#define TASKMANAGER TaskManager::getInstance()
#define DBMANAGER DBManager::getInstance()

//using namespace std;
