#include "stdafx.h"
#include "Logger.h"

//-----------------------------------------------------------------//
LogPrintf::LogPrintf()
{
	printf("* Log create : printf log mode\n");
}

LogPrintf::~LogPrintf()
{
}

void LogPrintf::log(const WCHAR *logStr)
{
	printf("%ws", logStr);
}

//-----------------------------------------------------------------//
LogFile::LogFile(xml_t *config)
{
	xmlNode_t *root = config->FirstChildElement("App")->FirstChildElement("Log");
	xmlNode_t *elem = root->FirstChildElement("Path");

	std::array<WCHAR, _MAX_PATH> logFilePath;
	StrConvA2W((char *)elem->GetText(), logFilePath.data(), logFilePath.max_size());
	printf("* Log create : [%ws]file log mode\n", logFilePath.data());
	this->initialize(logFilePath.data());
}

LogFile::~LogFile()
{
	fs_.close();
	fs_.clear();

	std::size_t found = fileName_.find(L".log");
	if (found == wstr_t::npos)
	{
		return;
	}

	//�ڿ� �α������� ������ �����, ����ð��� �����̸� �ڿ� �ٿ��ݴϴ�.
	wstr_t closeFileName = fileName_.substr(0, found);
	closeFileName += CLOCK.nowTime(L"_%Y%m%d-%H%M%S.log");
	_wrename(fileName_.c_str(), closeFileName.c_str());
}

void LogFile::initialize(WCHAR *logFileName)
{
	fileName_ = logFileName;
	fs_.imbue(std::locale("korean"));
	fs_.open(logFileName, std::ios::out | std::ios::trunc);
	
	if (fs_.bad())
	{
		printf("! logfile error, file open fail.\n");
		assert(false);
	}
}

void LogFile::log(const WCHAR *logStr)
{
	printf("%ws", logStr);

	fs_ << logStr;
	fs_.flush();
}

//-----------------------------------------------------------------//
LogWriter::LogWriter()
	:base_(nullptr)
{
}

LogWriter::~LogWriter()
{
	prefix_.clear();

	SAFE_DELETE(base_);
};

void LogWriter::setLogger(LogBase *base, const WCHAR *logPrefix)
{
	prefix_.clear();
	prefix_ = logPrefix;

	if (base_)
	{
		LogBase *old = base_;
		base_ = nullptr;
		//old->unInitialize();

		SAFE_DELETE(old);
	}

	base_ = base;
	// base_->initialize();
}

LogBase *LogWriter::logger() const
{
	return base_;
}

void LogWriter::log(const WCHAR *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	this->log(fmt, args);

	va_end(args);
}

void LogWriter::log(const WCHAR *fmt, va_list args)
{
	//���� �ð� �ֱ�
	wstr_t logMessage = CLOCK.nowTimeWithMilliSec();
	threadId_t threadId = GET_CURRENT_THREAD_ID();

	logMessage += L"\t";

	// ������ ���� �ֱ�
	Thread *thread = ThreadManager::getInstance().at(threadId);
	if (thread)
	{
		logMessage += thread->className();
	}
	else
	{
		logMessage += prefix_;
	}

	std::array<WCHAR, SIZE_8 * 2> threadIdStr;
	snwprintf(threadIdStr, L"0x%X", reinterpret_cast<UInt32&>(threadId));

	logMessage += L":";
	logMessage += threadIdStr.data();
	logMessage += L"\t";

	std::array<WCHAR, SIZE_1024> logStr;

	vswprintf_s(logStr.data(), logStr.size(), fmt, args);

	//base_�� �α׿� ����
	logMessage += logStr.data();
	logMessage += L"\n";
	base_->log((WCHAR *)logMessage.c_str());
}

//-----------------------------------------------------------------//
SystemLog::SystemLog()
{
	//config���� �Ľ�
	xml_t config;
	if (!loadConfig(&config))
	{
		printf("!!! have not config file\n");
		exit(0);
		return;
	}
	this->initialize(&config);
}

SystemLog::~SystemLog()
{
}

void SystemLog::initialize(xml_t *config)
{
	//config������ Log ã��
	xmlNode_t *root = config->FirstChildElement("App")->FirstChildElement("Log");
	if (!root)
	{
		printf("@ not exist log setting");
		LogBase *base = new LogPrintf();
		logWrite_.setLogger(base, L"testServer");
		return;
	}
	//Path ã��
	xmlNode_t *elem = root->FirstChildElement("Path");

	//prefix ã�� prefix����
	std::array<WCHAR, SIZE_256> tmp;
	elem = root->FirstChildElement("Prefix");
	StrConvA2W((char *)elem->GetText(), tmp.data(), tmp.max_size());
	wstr_t prefix = tmp.data();

	//Log Ÿ�� ã��
	LogBase *base;
	elem = root->FirstChildElement("Type");
	const char *type = (char *)elem->GetText();
	//WithFile Ÿ���̸� LogFile ���
	if (!strcmp(type, "WithFile"))
	{
		base = new LogFile(config);
	}
	//�ƴϸ� LogPrintf ���
	else
	{
		base = new LogPrintf();
	}

	//�α� base Ŭ������ prefix����
	logWrite_.setLogger(base, prefix.c_str());
}

void SystemLog::log(const WCHAR *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	logWrite_.log(fmt, args);
	va_end(args);
}
