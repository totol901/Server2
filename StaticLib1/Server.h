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

	/// config ���� ���� �ʱ�ȭ
	virtual void initialize(xml_t* config);

	/****************************************************************************
	�Լ���	: putPackage
	����		: package ����
	���ϰ�	: ����
	�Ű�����	: Package*
	*****************************************************************************/
	void					putPackage(Package *package);

	/// ���� run
	virtual bool run() = 0;

	/// ���� ���� get�Լ�
	SERVER_STATUS& status();
};

