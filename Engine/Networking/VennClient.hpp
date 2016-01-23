//---------------------------------------------------------------------------
// VennClient.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_VENNCLIENT_
#define _INCLUDED_VENNCLIENT_

#include "Engine/Networking/EnginePackets.hpp"

class Socket;

/////////////////////////////////////////////////////////////////////////////
class VennClient
{
public:
	VennClient( const char* ip, const char* port, bool isNonBlocking );
	~VennClient();

	void			Send( const char* buffer, size_t bufferLength, Channels channel );
	void			Receive();
	void			SendAck( int packetID );

	const Socket*	GetSocket() const { return m_socket; }

private:
	Socket*			m_socket;
	unsigned int	m_sentOrderedPacketId;
	unsigned int	m_sentReliablePacketId;
	unsigned int	m_expectedReliablePacketId;

private:
	PacketVector	m_unorderedPackets;
	PacketQueue		m_orderedPackets;
	PacketQueue		m_reliablePackets;

};

#endif