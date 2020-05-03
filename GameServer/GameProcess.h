#pragma once

class GameProcess : public ContentsProcess
{
private:
	void registSubPacketFunc();

	/// 초기화
	void initialize() override;
	///해제
	void release() override;

public:
	GameProcess();
	~GameProcess();

	static void C_REQ_JOIN_MAP(Session* session, Packet* rowPacket);
	static void C_MOVE_START_INPUT(Session* session, Packet* rowPacket);
};
