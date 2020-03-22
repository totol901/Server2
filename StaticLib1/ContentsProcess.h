#pragma once

// 컨텐츠를 여기서 파생해서 구현합니다.

#define MAX_PACKET_THREAD_		SIZE_64

class ContentsProcess
{
private:
	std::array<Thread *, MAX_PACKET_THREAD_> threadPool_;
	ThreadJobQueue<Package *> *packageQueue_;

protected:
	typedef void(*RunFunc)(Session *session, Packet *rowPacket);
	std::unordered_map<PacketType, RunFunc> runFuncTable_;

	/// 초기화
	virtual void initialize();
	///해제
	virtual void release();

private:
	/// 패킷 함수 등록 (주석 참고)
	void registDefaultPacketFunc();
	/// 패키지를 패킷 함수로 처리
	void run(Package *package);
	/// 패키지 처리
	void execute();

	/// 스래드 돌려줄 함수
	void process();
	
public:
	ContentsProcess();
	virtual ~ContentsProcess();

	

	/// 패키지 큐에 담는 함수
	void putPackage(Package *package);

	/// 서브 패킷 함수 등록
	virtual void registSubPacketFunc() = 0;

	//--------------------------------------------------------------//
	// 기본 패킷 기능 구현
	static void Packet_HeartBeat(Session *session, Packet *rowPacket);
	static void Packet_Notify_Terminal(Session *session, Packet *rowPacket);
	static void C_REQ_EXIT(Session *session, Packet *rowPacket);
};
