#pragma once

enum SERVER_STATUS
{
	SERVER_STOP,
	SERVER_INITIALZE,
	SERVER_READY
};

class Server
{
protected:
	char ip_[16];
	int port_;
	int workerThreadCount_;

	SERVER_STATUS status_;
	ContentsProcess* contentsProcess_;

public:
	Server(ContentsProcess* contentsProcess);
	virtual ~Server();

	/// config 파일 통해 초기화
	virtual void initialize(xml_t* config);

	/****************************************************************************
	함수명	: putPackage
	설명		: package 설정
	리턴값	: 없음
	매개변수	: Package*
	*****************************************************************************/
	void					putPackage(Package *package);

	/// 서버 run
	virtual bool run() = 0;

	/// 서버 상태 get함수
	SERVER_STATUS& status();
};

