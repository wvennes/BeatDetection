//---------------------------------------------------------------------------
// Socket.cpp
//---------------------------------------------------------------------------

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment( lib, "Ws2_32.lib" )

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Networking/Socket.hpp"

#define STATIC


//---------------------------------------------------------------------------
Socket::Socket()
	: m_socket( SOCKET_ERROR )
	, m_connectionInfo( nullptr )
{

}


//---------------------------------------------------------------------------
Socket::~Socket()
{
	Close();
}


//---------------------------------------------------------------------------
STATIC void Socket::Initialize()
{
	WSAData wsaData;
	int status = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
	if ( status != 0 )
	{
		DebuggerPrintf( "WSAStartup error : %d\n", status );
	}
}


//---------------------------------------------------------------------------
STATIC void Socket::Shutdown()
{
	WSACleanup();
}


//---------------------------------------------------------------------------
void Socket::Create( const char* ip, const char* port, bool isNonBlocking )
{
	addrinfo hints;
	u_long blockMode = 1;

	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	///if ( m_isUsingUdp ) hints.ai_socktype = SOCK_DGRAM;
	///else hints.ai_socktype = SOCK_STREAM;
	int status = getaddrinfo( ip, port, &hints, ( addrinfo** ) &m_connectionInfo );
	if ( status != 0 )
	{
		DebuggerPrintf( "getaddrinfo error: %d\n", WSAGetLastError() );
		return;
	}

	// make socket
	addrinfo* tempServerInfo = ( addrinfo* ) m_connectionInfo;
	m_socket = socket( tempServerInfo->ai_family, tempServerInfo->ai_socktype, tempServerInfo->ai_protocol );
	if ( m_socket == INVALID_SOCKET )
	{
		printf( "socket creation failed: %d\n", WSAGetLastError() );
		return;
	}

	// make non-blocking
	if ( isNonBlocking )
	{
		status = ioctlsocket( m_socket, FIONBIO, &blockMode );
		if ( status == INVALID_SOCKET )
		{
			printf( "ioctlsocket error: %d\n", WSAGetLastError() );
			return;
		}
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
void Socket::Bind()
{
	addrinfo* info = ( addrinfo* ) m_connectionInfo;
	bind( m_socket, info->ai_addr, info->ai_addrlen );
}


//---------------------------------------------------------------------------
bool Socket::Send( const char* buffer, size_t bufferLength )
{
	addrinfo* info = ( addrinfo* ) m_connectionInfo;
	return SendToSockaddr( buffer, bufferLength, info->ai_addr, info->ai_addrlen );
}


//---------------------------------------------------------------------------
bool Socket::SendToSockaddr( const char* buffer, size_t bufferLength, void* sockaddrReceiver, int sockaddrLength )
{
	sockaddr* address = ( sockaddr* ) sockaddrReceiver;
	int status = sendto( m_socket, buffer, bufferLength, 0, address, sockaddrLength );
	if ( status < 0 )
	{
		printf( "Socket send error: %d\n", WSAGetLastError() );
		return false;
	}

	return true;
}


//---------------------------------------------------------------------------
bool Socket::Receive( char* buffer, size_t bufferLength )
{
	addrinfo* info = ( addrinfo* ) m_connectionInfo;
	return ReceiveFromSockaddr( buffer, bufferLength, info->ai_addr, info->ai_addrlen );
}


//---------------------------------------------------------------------------
bool Socket::ReceiveFromSockaddr( char* buffer, size_t bufferLength, void* sockaddrSender, int sockaddrLength )
{
	sockaddr* address = ( sockaddr* ) sockaddrSender;
	int status = recvfrom( m_socket, buffer, bufferLength, 0, address, &sockaddrLength );
	if ( status < 0 )
	{
		printf( "Socket receive error: %d\n", WSAGetLastError() );
		return false;
	}
	else if ( WSAGetLastError() == WSAEWOULDBLOCK )
	{
		// do nothing
		return false;
	}

	return true;
}


//---------------------------------------------------------------------------
void Socket::Close()
{
	closesocket( m_socket );
	freeaddrinfo( ( addrinfo* ) m_connectionInfo );
}