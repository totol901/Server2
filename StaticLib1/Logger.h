#pragma once

// TODO : unInitialize�̰� ���� �����, �� �� ¥������ ����, ��� �߰��� �����غ���

/*
! : �߿䵵 ����
* : �߿䵵 �Ϲ�
# : ����
@ : ��Ÿ
*/

/****************************************************************************
��ũ�θ�	: SLog
����		: fmt�� �´� ���ڸ� cmd�� �α����Ͽ� ��
�Ű�����	: WCHAR*, ...
*****************************************************************************/
#define SLog(arg, ...)				SystemLog::getInstance().log(arg, __VA_ARGS__);

/****************************************************************************
��ũ�θ�	: SErrLog
����		: fmt�� �´� ���ڸ� cmd�� �α����Ͽ� �� ��, ���μ��� ����
�Ű�����	: WCHAR*, ...
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
	�Լ���	: log
	����		: cmdâ�� logStr �α� ����
	���ϰ�	: ����
	�Ű�����	: WCHAR*
	*****************************************************************************/
	void log(const WCHAR *logStr);
};

//-----------------------------------------------------------------//
class LogFile : public LogBase
{
private:
	std::wfstream   fs_;
	wstr_t			fileName_;

	//logFileName��ġ�� �α� ������ ������
	void initialize(WCHAR *logFileName);

public:
	LogFile(xml_t *config);
	~LogFile();

	/****************************************************************************
	�Լ���	: log
	����		: logStr�� �α����ϰ� cmdâ�� ��
	���ϰ�	: ����
	�Ű�����	: WCHAR*
	*****************************************************************************/
	void log(const WCHAR *logStr);
};

//-----------------------------------------------------------------//
// �α� ���� ��ü
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
	�Լ���	: log
	����		: fmt�� �´� ���ڸ� base_�� ���� log�� ��
	���ϰ�	: ����
	�Ű�����	: WCHAR*, ...
	*****************************************************************************/
	void log(const WCHAR *fmt, ...);
	void log(const WCHAR *fmt, va_list args);

public:
	//set�Լ�
	void setLogger(LogBase *base, const WCHAR *logPrefix);
	//get�Լ�
	LogBase *logger() const;
};

typedef LogWriter* LogWriterPtr;

//-----------------------------------------------------------------//
// ���� �ý��� �α�
//-----------------------------------------------------------------//
class SystemLog : public Singleton<SystemLog>
{
	friend Singleton;
private:
	LogWriter   logWrite_;

	//config������ �ҷ��� ���ϴ� �α� ���¸� ���ϰ� Ŭ���� �ʱ�ȭ��
	void initialize(xml_t *config);

	SystemLog();
	virtual ~SystemLog();

public:
	

	/****************************************************************************
	�Լ���	: log
	����		: fmt�� �´� ���ڸ� base_�� ���� log�� ��
	���ϰ�	: ����
	�Ű�����	: WCHAR*, ...
	*****************************************************************************/
	void log(const WCHAR *fmt, ...);
};

