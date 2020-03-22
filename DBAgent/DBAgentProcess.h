#pragma once

class DBAgentProcess : public ContentsProcess
{
public:
	DBAgentProcess();
	~DBAgentProcess();

	/// 초기화
	void initialize() override;
	///해제
	void release() override;

private:
	void registSubPacketFunc();

	static void I_DB_REQ_ID_PW(Session *session, Packet *rowPacket);
	static void I_DB_REQ_LOAD_DATA(Session *session, Packet *rowPacket);
};
