//---------------------------------------------------------------------------
// Server.cpp
//---------------------------------------------------------------------------

//#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment( lib, "Ws2_32.lib" )

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Networking/Server.hpp"


//---------------------------------------------------------------------------
Server::Server( const char* port/*, bool isUsingUdp*/ )
	: m_serverSocket( INVALID_SOCKET )
	//, m_remoteSocket( INVALID_SOCKET )
	, m_serverInfo( nullptr )
	, m_remoteInfo( nullptr )
	//, m_isUsingUdp( isUsingUdp )
{
	WSAData wsaData;
	addrinfo hints;
	u_long blockingMode = 1;

	// initialize
	int status = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
	if ( status != 0 )
	{
		printf( "WSAStartup error: %d\n", status );
		return;
	}

	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	//if ( m_isUsingUdp ) hints.ai_socktype = SOCK_DGRAM;
	//else hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use our current IP for the server

	getaddrinfo( NULL, port, &hints, ( addrinfo** ) &m_serverInfo );

	addrinfo* tempServerInfo = ( addrinfo* ) m_serverInfo;

	// make a socket
	m_serverSocket = socket( tempServerInfo->ai_family, tempServerInfo->ai_socktype, tempServerInfo->ai_protocol );

	// make non-blocking
	status = ioctlsocket( m_serverSocket, FIONBIO, &blockingMode );
	if ( status == INVALID_SOCKET )
	{
		printf( "ioctlsocket error: %d\n", WSAGetLastError() );
		return;
	}

	// bind the socket
	bind( m_serverSocket, tempServerInfo->ai_addr, tempServerInfo->ai_addrlen );

	// listen on this socket
	//listen( m_serverSocket, 10 );
}


//---------------------------------------------------------------------------
Server::~Server()
{
	closesocket( m_serverSocket );
	freeaddrinfo( ( addrinfo* ) m_serverInfo );
	WSACleanup();
}


//---------------------------------------------------------------------------
bool Server::Send( const char* buffer, int bufferLength, int flags )
{
	return SendUDP( buffer, bufferLength, flags );
}


//---------------------------------------------------------------------------
bool Server::Receive( char* buffer, int bufferLength, int flags )
{
	return ReceiveUDP( buffer, bufferLength, flags );
}


//---------------------------------------------------------------------------
bool Server::SendUDP( const char* buffer, int bufferLength, int flags )
{
	sockaddr_storage remoteAddress;
	int remoteAddressLength = sizeof( remoteAddress );
	int status = sendto( m_serverSocket, buffer, bufferLength, flags, ( sockaddr* ) &remoteAddress, remoteAddressLength );
	if ( status < 0 )
	{
		printf( "sendto error: %d\n", WSAGetLastError() );
		return false;
	}
	return true;
}


//---------------------------------------------------------------------------
bool Server::SendToAddress( const char* buffer, int bufferLength, int flags, sockaddr* remoteAddress, int remoteAddressLength )
{
	int status = sendto( m_serverSocket, buffer, bufferLength, flags, remoteAddress, remoteAddressLength );
	if ( status < 0 )
	{
		printf( "sendto error: %d\n", WSAGetLastError() );
		return false;
	}
	return true;
}


//---------------------------------------------------------------------------
bool Server::ReceiveUDP( char* buffer, int bufferLength, int flags )
{
	sockaddr remoteAddress;
	int remoteAddressLength = sizeof( remoteAddress );
	memset( buffer, 0, bufferLength );
	int status = recvfrom( m_serverSocket, buffer, bufferLength, flags, &remoteAddress, &remoteAddressLength );

	if ( WSAGetLastError() == WSAEWOULDBLOCK ) return false;
	else if ( status < 0 )
	{
		printf( "recvfrom error: %d\n", WSAGetLastError() );
		return false;
	}

	status = sendto( m_serverSocket, buffer, bufferLength, 0, ( sockaddr* ) &remoteAddress, remoteAddressLength );
	printf( "Echoing message...\n" );
	return true;
}


//---------------------------------------------------------------------------
bool Server::ReceiveFromAddress( char* buffer, int bufferLength, int flags, sockaddr* remoteAddress, int* remoteAddressLength )
{
	memset( buffer, 0, bufferLength );
	int status = recvfrom( m_serverSocket, buffer, bufferLength, flags, remoteAddress, remoteAddressLength );

	if ( WSAGetLastError() == WSAEWOULDBLOCK || status < 0 ) return false;
	return true;
}