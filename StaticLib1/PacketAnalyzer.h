#pragma once

class PacketAnalyzer : public Singleton < PacketAnalyzer >
{
	friend Singleton;
public:
	/// ��Ŷ �м� �ϴ� �Լ�
	Packet* analyzer(const char *rowPacket, size_t size);
};
