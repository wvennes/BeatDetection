//---------------------------------------------------------------------------
// Socket.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_SOCKET_
#define _INCLUDED_SOCKET_

/////////////////////////////////////////////////////////////////////////////
class Socket
{
public:
	Socket();
	~Socket();

	static void		Initialize();
	static void		Shutdown();
	void			Create( const char* ip, const char* port, bool isNonBlocking );
	void			Bind();
	bool			Send( const char* buffer, size_t bufferLength );
	bool			SendToSockaddr( const char* buffer, size_t bufferLength, void* sockaddrReceiver, int sockaddrLength );
	bool			Receive( char* buffer, size_t bufferLength );
	bool			ReceiveFromSockaddr( char* buffer, size_t bufferLength, void* sockaddrSender, int sockaddrLength );
	void			Close();

private:
	int				m_socket;
	void*			m_connectionInfo;

};

#endif