#pragma once

class PacketAnalyzer : public Singleton < PacketAnalyzer >
{
	friend Singleton;
public:
	/// 패킷 분석 하는 함수
	Packet* analyzer(const char *rowPacket, size_t size);
};
