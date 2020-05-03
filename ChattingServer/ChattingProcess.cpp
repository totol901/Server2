#include "pch.h"
#include "ChattingProcess.h"

ChattingProcess::ChattingProcess()
	:ContentsProcess()
{
	this->initialize();
}

ChattingProcess::~ChattingProcess()
{
}

void ChattingProcess::initialize()
{
	ContentsProcess::initialize();

	this->registSubPacketFunc();
}

void ChattingProcess::release()
{
	ContentsProcess::release();
}

void ChattingProcess::registSubPacketFunc()
{
#define INSERT_PACKET_PROCESS(type)		runFuncTable_.insert(std::make_pair(E_##type, &ChattingProcess::##type))

	INSERT_PACKET_PROCESS(I_CHTTING_NOTIFY_ID);
	INSERT_PACKET_PROCESS(I_DB_ANS_PARSE_DATA);
	INSERT_PACKET_PROCESS(C_REQ_REGIST_CHATTING_NAME);
	INSERT_PACKET_PROCESS(C_REQ_CHATTING);
	INSERT_PACKET_PROCESS(C_REQ_EXIT);
}

//---------------------------------------------------------------//
//패킷 처리 정의
void ChattingProcess::I_CHTTING_NOTIFY_ID(Session *session, Packet *rowPacket)
{
	PK_I_CHTTING_NOTIFY_ID *packet = (PK_I_CHTTING_NOTIFY_ID *)rowPacket;

	PK_I_DB_REQ_LOAD_DATA dbPacket;
	dbPacket.clientId_ = packet->clientId_;
	dbPacket.oidAccountId_ = packet->oidAccountId_;

	Terminal *terminal = TERMINALMANAGER.get(L"DBAgent");
	terminal->sendPacket(&dbPacket);
}

void ChattingProcess::I_DB_ANS_PARSE_DATA(Session *session, Packet *rowPacket)
{
	PK_I_DB_ANS_PARSE_DATA *packet = (PK_I_DB_ANS_PARSE_DATA  *)rowPacket;

	PK_I_LOGIN_NOTIFY_ID_LOADED iPacket;
	iPacket.clientId_ = packet->clientId_;
	iPacket.name_ = packet->name_;
	iPacket.result_ = packet->result_;

	SLog(L"* [%S] name load from db", iPacket.name_.c_str());
	Terminal *terminal = TERMINALMANAGER.get(L"LoginServer");
	terminal->sendPacket(&iPacket);
}

void ChattingProcess::C_REQ_REGIST_CHATTING_NAME(Session *session, Packet *rowPacket)
{
	PK_C_REQ_REGIST_CHATTING_NAME *packet = (PK_C_REQ_REGIST_CHATTING_NAME *)rowPacket;
	User *user = USERMANAGER.at(session->id());
	if (user != nullptr)
	{
		SLog(L"! try duplicate regist : %s, name : %S", session->clientAddress().c_str(), packet->name_.c_str());
		session->onClose(true);
		return;
	}

	user = new User(session);
	user->setName(packet->name_);
	USERMANAGER.insert(user);

	SLog(L"* user [%S] created from [%s]", packet->name_.c_str(), session->clientAddress().c_str());
}
 
void ChattingProcess::C_REQ_CHATTING(Session *session, Packet *rowPacket)
{
	PK_C_REQ_CHATTING *packet = (PK_C_REQ_CHATTING *)rowPacket;

	User *user = USERMANAGER.at(session->id());
	if (user == nullptr) 
	{
		SLog(L"! not registed : %s", session->clientAddress().c_str());
		session->onClose(true);
		return;
	}

	PK_S_ANS_CHATTING* retPacket = new PK_S_ANS_CHATTING();
	retPacket->name_ = user->name();
	retPacket->text_ = packet->text_;

	SLog(L"* send message %S, %S", retPacket->name_.c_str(), retPacket->text_.c_str());
	//session->sendPacket(&retPacket);
	USERMANAGER.sendMessageAllUser(retPacket);
}

void ChattingProcess::C_REQ_EXIT(Session *session, Packet *rowPacket)
{
	//클라이언트 read thread 를 종료시켜 주기 위해 처리
	PK_C_REQ_EXIT *packet = (PK_C_REQ_EXIT *)rowPacket;
	User *user = USERMANAGER.at(session->id());
	if (user == nullptr) 
	{
		SLog(L"! not registed : %s", session->clientAddress().c_str());
		session->onClose();
		return;
	}
	USERMANAGER.remove(session->id());

	PK_S_ANS_EXIT* ansPacket = new PK_S_ANS_EXIT();
	SLog(L"* recv exit packet by [%s]", session->clientAddress().c_str());
	session->sendPacket(ansPacket);
}
