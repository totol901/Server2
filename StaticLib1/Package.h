#pragma once

class Session;
class Package
{
public:
	Session *session_;
	Packet *packet_;

	Package(Session *session, Packet *packet)
	{
		session_ = session;
		packet_ = packet;
	}

	~Package()
	{
		session_->DecreseReferenceCount();
		session_ = nullptr;
		SAFE_DELETE(packet_);
	}
};