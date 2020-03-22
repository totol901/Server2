#include "stdafx.h"
#include "PacketAnalyzer.h"

Packet* PacketAnalyzer::analyzer(const char *rowPacket, size_t size)
{
	size_t offset = 0;
	PacketType type[1] = { (PacketType)0, };
	memcpy_s((void *)type, sizeof(type), (void *)rowPacket, sizeof(Int64));
	offset += sizeof(type);

	Packet *packet = PACKETFACTORY.getPacket(type[0]);

	if (packet)
	{
		if (offset < size)
		{
			Stream stream((UCHAR *)(rowPacket + offset), size - offset);
			packet->decode(stream);
		}
	}

	return packet;
}
