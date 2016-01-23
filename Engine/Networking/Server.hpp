//---------------------------------------------------------------------------
// Server.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_SERVER_
#define _INCLUDED_SERVER_

#include <WinSock2.h>

/////////////////////////////////////////////////////////////////////////////
class Server
{
public:
	Server( const char* port/*, bool isUsingUdp*/ );
	~Server();

	virtual bool			Send( const char* buffer, int bufferLength, int flags );
	virtual bool			Receive( char* buffer, int bufferLength, int flags );

protected:
	unsigned int			m_serverSocket;
	//unsigned int			m_remoteSocket;
	void*					m_serverInfo;
	void*					m_remoteInfo;
	//bool					m_isUsingUdp;

protected:
	//bool					SendTCP( const char* buffer, int bufferLength, int flags );
	virtual bool			SendUDP( const char* buffer, int bufferLength, int flags );
	virtual bool			SendToAddress( const char* buffer, int bufferLength, int flags, sockaddr* remoteAddress, int remoteAddressLength );
	//bool					ReceiveTCP( char* buffer, int bufferLength, int flags );
	virtual bool			ReceiveUDP( char* buffer, int bufferLength, int flags );
	virtual bool			ReceiveFromAddress( char* buffer, int bufferLength, int flags, sockaddr* remoteAddress, int* remoteAddressLength );

};

#endif