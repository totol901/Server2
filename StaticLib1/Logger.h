#pragma once

// TODO : unInitialize이거 쓸곳 없어보임, 꽤 잘 짜여진거 같음, 기능 추가도 가능해보임

/*
! : 중요도 높음
* : 중요도 일반
# : 낮음
@ : 기타
*/

/****************************************************************************
매크로명	: SLog
설명		: fmt에 맞는 문자를 cmd나 로그파일에 씀
매개변수	: WCHAR*, ...
*****************************************************************************/
#define SLog(arg, ...)				SystemLog::getInstance().log(arg, __VA_ARGS__);

/****************************************************************************
매크로명	: SErrLog
설명		: fmt에 맞는 문자를 cmd나 로그파일에 쓴 뒤, 프로세스 종료
매개변수	: WCHAR*, ...
*****************************************************************************/
#define SErrLog(arg, ...)			SystemLog::getInstance().log(arg, __VA_ARGS__); ::ExitProcess(0);

//-----------------------------------------------------------------//
class LogBase
{
public:
	LogBase() {}
	virtual ~LogBase() {}

	//virtual void initialize() {}
	//virtual void unInitialize() {}
	virtual void log(const WCHAR *logStr) = 0;
};

//-----------------------------------------------------------------//
class LogPrintf : public LogBase
{
public:
	LogPrintf();
	~LogPrintf();

	/****************************************************************************
	함수명	: log
	설명		: cmd창에 logStr 로그 남김
	리턴값	: 없음
	매개변수	: WCHAR*
	*****************************************************************************/
	void log(const WCHAR *logStr);
};

//-----------------------------------------------------------------//
class LogFile : public LogBase
{
private:
	std::wfstream   fs_;
	wstr_t			fileName_;

	//logFileName위치에 로그 파일을 생성함
	void initialize(WCHAR *logFileName);

public:
	LogFile(xml_t *config);
	~LogFile();

	/****************************************************************************
	함수명	: log
	설명		: logStr을 로그파일과 cmd창에 씀
	리턴값	: 없음
	매개변수	: WCHAR*
	*****************************************************************************/
	void log(const WCHAR *logStr);
};

//-----------------------------------------------------------------//
// 로그 쓰는 주체
//-----------------------------------------------------------------//
class LogWriter
{
private:
	LogBase			*base_;
	wstr_t			prefix_;

public:
	LogWriter();
	~LogWriter();

	/****************************************************************************
	함수명	: log
	설명		: fmt에 맞는 문자를 base_를 통해 log에 씀
	리턴값	: 없음
	매개변수	: WCHAR*, ...
	*****************************************************************************/
	void log(const WCHAR *fmt, ...);
	void log(const WCHAR *fmt, va_list args);

public:
	//set함수
	void setLogger(LogBase *base, const WCHAR *logPrefix);
	//get함수
	LogBase *logger() const;
};

typedef LogWriter* LogWriterPtr;

//-----------------------------------------------------------------//
// 어플 시스템 로그
//-----------------------------------------------------------------//
class SystemLog : public Singleton<SystemLog>
{
	friend Singleton;
private:
	LogWriter   logWrite_;

	//config파일을 불러와 원하는 로그 형태를 정하고 클래스 초기화함
	void initialize(xml_t *config);

	SystemLog();
	virtual ~SystemLog();

public:
	

	/****************************************************************************
	함수명	: log
	설명		: fmt에 맞는 문자를 base_를 통해 log에 씀
	리턴값	: 없음
	매개변수	: WCHAR*, ...
	*****************************************************************************/
	void log(const WCHAR *fmt, ...);
};

