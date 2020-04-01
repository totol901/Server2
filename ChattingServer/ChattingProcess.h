#pragma once

class ChattingProcess : public ContentsProcess
{
private:
	void registSubPacketFunc();

	/// 초기화
	void initialize() override;
	///해제
	void release() override;

public:
	ChattingProcess();
	~ChattingProcess();

	static void I_CHTTING_NOTIFY_ID(Session *session, Packet *rowPacket);
	static void I_DB_ANS_PARSE_DATA(Session *session, Packet *rowPacket);
	static void C_REQ_REGIST_CHATTING_NAME(Session *session, Packet *rowPacket);
	static void C_REQ_CHATTING(Session *session, Packet *rowPacket);
	static void C_REQ_EXIT(Session *session, Packet *rowPacket);
};
