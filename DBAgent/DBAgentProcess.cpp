#include "pch.h"
#include "DBAgentProcess.h"

DBAgentProcess::DBAgentProcess()
	:ContentsProcess()
{
	this->initialize();
}

DBAgentProcess::~DBAgentProcess()
{
}

void DBAgentProcess::initialize()
{
	ContentsProcess::initialize();

	this->registSubPacketFunc();
}

void DBAgentProcess::release()
{
	ContentsProcess::release();
}

void DBAgentProcess::registSubPacketFunc()
{
#define INSERT_PACKET_PROCESS(type)		runFuncTable_.insert(std::make_pair(E_##type, &DBAgentProcess::##type))

	INSERT_PACKET_PROCESS(I_DB_REQ_ID_PW);
	INSERT_PACKET_PROCESS(I_DB_REQ_CREATE_ID);
	INSERT_PACKET_PROCESS(I_DB_REQ_LOAD_DATA);
	INSERT_PACKET_PROCESS(I_DB_REQ_LOGOUT);
}

void DBAgentProcess::I_DB_REQ_ID_PW(Session *session, Packet *rowPacket)
{
	PK_I_DB_REQ_ID_PW *packet = (PK_I_DB_REQ_ID_PW *)rowPacket;

	QI_DB_REQ_ID_PW *query = new QI_DB_REQ_ID_PW();	        			// 쿼리 만들기
	query->clientId_ = packet->clientId_;

	QueryStatement *statement = query->statement();
	statement->addParam((char *)packet->id_.c_str());					// 파라메터 계속 붙여나가기
	statement->addParam((char *)packet->password_.c_str());

	DBMANAGER.pushQuery(query);
}

void DBAgentProcess::I_DB_REQ_CREATE_ID(Session* session, Packet* rowPacket)
{
	PK_I_DB_REQ_CREATE_ID* packet = (PK_I_DB_REQ_CREATE_ID*)rowPacket;

	QI_DB_REQ_CREATE_ID* query = new QI_DB_REQ_CREATE_ID();
	query->clientId_ = packet->clientId_;

	QueryStatement* statement = query->statement();
	statement->addParam((char*)packet->id_.c_str());					// 파라메터 계속 붙여나가기
	statement->addParam((char*)packet->password_.c_str());
	statement->addParam((char*)packet->name_.c_str());

	DBMANAGER.pushQuery(query);
}

void DBAgentProcess::I_DB_REQ_LOAD_DATA(Session *session, Packet *rowPacket)
{
	PK_I_DB_REQ_LOAD_DATA *packet = (PK_I_DB_REQ_LOAD_DATA *)rowPacket;

	QI_DB_REQ_LOAD_DATA *query = new QI_DB_REQ_LOAD_DATA();
	query->clientId_ = packet->clientId_;

	QueryStatement *statement = query->statement();
	statement->addParam(packet->oidAccountId_);

	DBMANAGER.pushQuery(query);
}

void DBAgentProcess::I_DB_REQ_LOGOUT(Session* session, Packet* rowPacket)
{
	PK_I_DB_REQ_LOGOUT* packet = (PK_I_DB_REQ_LOGOUT*)rowPacket;

	QI_DB_REQ_LOGOUT* query = new QI_DB_REQ_LOGOUT();

	QueryStatement* statement = query->statement();
	statement->addParam((char*)packet->name_.c_str());

	DBMANAGER.pushQuery(query);
}
