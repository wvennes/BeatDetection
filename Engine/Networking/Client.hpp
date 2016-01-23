//---------------------------------------------------------------------------
// Client.hpp
// - create a TCP or UDP client based on port and ip address
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_CLIENT_
#define _INCLUDED_CLIENT_

/////////////////////////////////////////////////////////////////////////////
class Client
{
public:
	Client( const char* ip, const char* port/*, bool isUsingUdp*/ );
	~Client();

	unsigned int	GetSocket() const { return m_socket; }
	bool			Send( const char* buffer, int bufferLength, int flags );
	bool			Receive( char* buffer, int bufferLength, int flags );

private:
	unsigned int	m_socket;
	void*			m_serverInfo;
	//bool			m_isUsingUdp;

private:
	bool			SendUDP( const char* buffer, int bufferLength, int flags );
	//bool			SendTCP( const char* buffer, int bufferLength, int flags );
	bool			ReceiveUDP( char* buffer, int bufferLength, int flags );
	//bool			ReceiveTCP( char* buffer, int bufferLength, int flags );

};

#endif