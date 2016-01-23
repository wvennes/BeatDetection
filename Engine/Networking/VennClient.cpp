//---------------------------------------------------------------------------
// VennClient.cpp
//---------------------------------------------------------------------------

#include "Engine/Networking/Socket.hpp"
#include "Engine/Networking/VennClient.hpp"


//---------------------------------------------------------------------------
VennClient::VennClient( const char* ip, const char* port, bool isNonBlocking )
	: m_socket( nullptr )
{
	Socket::Initialize();
	m_socket = new Socket();
	m_socket->Create( ip, port, isNonBlocking );
}


//---------------------------------------------------------------------------
VennClient::~VennClient()
{
	if ( m_socket != nullptr ) delete m_socket;
}


//---------------------------------------------------------------------------
void VennClient::Receive()
{

}


//---------------------------------------------------------------------------
void VennClient::Send( const char* buffer, size_t bufferLength, Channels channel )
{
	EnginePacket::Packet p;
	p.channelID = ( char ) channel;
	switch ( channel )
	{
		case CH_UNRELIABLE:
		{
			p.packetID = 0;
			break;
		}
		case CH_ORDERED:
		{
			p.packetID = m_sentOrderedPacketId;
			++ m_sentOrderedPacketId;
			break;
		}
		case CH_RELIABLE:
		{
			p.packetID = m_sentReliablePacketId;
			++ m_sentReliablePacketId;
			break;
		}
	}

	char* temp = new char[ sizeof( p ) + bufferLength ];
	memcpy( temp, &p, sizeof( p ) );
	memcpy( temp + sizeof( p ), buffer, bufferLength );
	m_socket->Send( temp, sizeof( p ) + bufferLength );
	delete temp;
}


//---------------------------------------------------------------------------
void VennClient::SendAck( int packetID )
{
	EnginePacket p;
	p.p.channelID = ( char ) CH_ACK;
	p.p.packetID = packetID;

	char buffer[ sizeof( p.p ) ];
	EnginePacket* temp = ( EnginePacket* ) buffer;
	temp->p.channelID = p.p.channelID;
	temp->p.packetID = p.p.packetID;

	m_socket->Send( buffer, sizeof( p.p ) );
}