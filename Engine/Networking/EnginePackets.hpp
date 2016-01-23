//---------------------------------------------------------------------------
// EnginePackets.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_ENGINEPACKETS_
#define _INCLUDED_ENGINEPACKETS_

#include <queue>
#include <vector>

const size_t	MAX_PACKET_SIZE = 512;

/////////////////////////////////////////////////////////////////////////////
enum Channels
{
	CH_UNRELIABLE	= 0,
	CH_ORDERED		= 1,
	CH_RELIABLE		= 2,
	CH_ACK			= 3,
};


////////////////////////////////////////////////////////////////////////////
union EnginePacket
{
	struct Packet
	{
		char	channelID;
		int		packetID;
	} p;

	char buffer[ MAX_PACKET_SIZE ];
};


/////////////////////////////////////////////////////////////////////////////
struct EnginePacketComparison
{
	bool operator()( const EnginePacket& lhs, const EnginePacket& rhs )
	{
		return lhs.p.packetID > rhs.p.packetID;
	}
};

typedef	std::vector< EnginePacket >																	PacketVector;
typedef	std::priority_queue< EnginePacket, std::vector< EnginePacket >, EnginePacketComparison >	PacketQueue;

#endif