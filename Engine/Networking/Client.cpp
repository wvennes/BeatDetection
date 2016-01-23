//---------------------------------------------------------------------------
// Client.cpp
//---------------------------------------------------------------------------

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment( lib, "Ws2_32.lib" )

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Networking/Client.hpp"


//---------------------------------------------------------------------------
Client::Client( const char* ip, const char* port/*, bool isUsingUdp*/ )
	: m_socket( INVALID_SOCKET )
	, m_serverInfo( nullptr )
	//, m_isUsingUdp( isUsingUdp )
{
	WSAData wsaData;
	addrinfo hints;
	u_long blockMode = 1;
	int status = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
	if ( status != 0 )
	{
		DebuggerPrintf( "WSAStartup error : %d\n", status );
		return;
	}

	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	///if ( m_isUsingUdp ) hints.ai_socktype = SOCK_DGRAM;
	///else hints.ai_socktype = SOCK_STREAM;
	status = getaddrinfo( ip, port, &hints, ( addrinfo** ) &m_serverInfo );
	if ( status != 0 )
	{
		DebuggerPrintf( "getaddrinfo error: %d\n", WSAGetLastError() );
		return;
	}

	// make socket
	addrinfo* tempServerInfo = ( addrinfo* ) m_serverInfo;
	m_socket = socket( tempServerInfo->ai_family, tempServerInfo->ai_socktype, tempServerInfo->ai_protocol );
	if ( m_socket == INVALID_SOCKET )
	{
		printf( "socket creation failed: %d\n", WSAGetLastError() );
		return;
	}

	// make non-blocking
	status = ioctlsocket( m_socket, FIONBIO, &blockMode );
	if ( status == INVALID_SOCKET )
	{
		printf( "ioctlsocket error: %d\n", WSAGetLastError() );
		return;
	}

	// call connect if TCP socket
// 	if ( !isUsingUdp )
// 	{
// 		status = connect( m_socket, tempServerInfo->ai_addr, tempServerInfo->ai_addrlen );
// 		if ( status < 0 )
// 		{
// 			printf( "connect failed: %d\n", WSAGetLastError() );
// 			return;
// 		}
// 	}
}


//---------------------------------------------------------------------------
Client::~Client()
{
	closesocket( m_socket );
	freeaddrinfo( ( addrinfo* ) m_serverInfo );
	WSACleanup();
}


//---------------------------------------------------------------------------
bool Client::Send( const char* buffer, int bufferLength, int flags )
{
	return SendUDP( buffer, bufferLength, flags );
	//if ( m_isUsingUdp ) return SendUDP( buffer, bufferLength, flags );
	//else return SendTCP( buffer, bufferLength, flags );
}


//---------------------------------------------------------------------------
bool Client::SendUDP( const char* buffer, int bufferLength, int flags )
{
	addrinfo* tempServerInfo = ( addrinfo* ) m_serverInfo;
	int status = sendto( m_socket, buffer, bufferLength, flags, tempServerInfo->ai_addr, tempServerInfo->ai_addrlen );
	if ( status < 0 )
	{
		printf( "Sendto error: %d\n", WSAGetLastError() );
		return false;
	}
	return true;
}


//---------------------------------------------------------------------------
//bool Client::SendTCP( const char* buffer, int bufferLength, int flags )
//{
//	int status = send( m_socket, buffer, bufferLength, flags );
//	if ( status != 0 )
//	{
//		printf( "Send error: %d\n", WSAGetLastError() );
//		return false;
//	}
//	return true;
//}


//---------------------------------------------------------------------------
bool Client::Receive( char* buffer, int bufferLength, int flags )
{
	return ReceiveUDP( buffer, bufferLength, flags );
	//if ( m_isUsingUdp ) return ReceiveUDP( buffer, bufferLength, flags );
	//else return ReceiveTCP( buffer, bufferLength, flags );
}


//---------------------------------------------------------------------------
bool Client::ReceiveUDP( char* buffer, int bufferLength, int flags )
{
	int fromLength = sizeof( sockaddr_storage );
	memset( buffer, 0, bufferLength );
	addrinfo* tempServerInfo = ( addrinfo* ) m_serverInfo;
	int status = recvfrom( m_socket, buffer, bufferLength, flags, tempServerInfo->ai_addr, &fromLength );
	if ( WSAGetLastError() == WSAEWOULDBLOCK )
	{
		return false;
	}
	else if ( status < 0 )
	{
		//printf( "recv error: %d\n", WSAGetLastError() );
		return false;
	}
	return true;
}


//---------------------------------------------------------------------------
//bool Client::ReceiveTCP( char* buffer, int bufferLength, int flags )
//{
//	ZeroMemory( buffer, bufferLength );
//	int status = recv( m_socket, buffer, bufferLength, flags );
//	if ( status != 0 )
//	{
//		printf( "recv error: %d\n", WSAGetLastError() );
//		return false;
//	}
//	return true;
//}