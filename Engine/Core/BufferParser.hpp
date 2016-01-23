//---------------------------------------------------------------------------
// BufferParser.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_BUFFERPARSER_
#define _INCLUDED_BUFFERPARSER_

#include <vector>
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Rgba.hpp"

class BufferParser
{
public:
	BufferParser( const unsigned char* scanBegin, const size_t bufferSize, const bool isBinary );
	BufferParser( std::vector< unsigned char >& outBuffer, const bool isBinary );
	~BufferParser();

	bool							IsScanBeyondBuffer() const;
	void							AdvancePastString();

	static void						AdvanceToWhiteSpace( const char*& scan );
	static void						AdvanceToNextLine( const char*& scan );
	static void						AdvanceToNextNonWhitespace( const char*& scan );
	static void						AdvanceToToken( const char*& scan, const char token );
	static bool						IsEqualToString( const char* begin, const char* end, const char* comparison );
	static void						CopyStringToBuffer( const char* begin, const char* end, char* const buffer );
	static bool						IsTokenInString( const char* begin, const char* end, const char token );
	static int						GetNumberOfStringsInLine( const char* begin );

	const unsigned char* const		GetReadScan();

	unsigned char					ReadUnsignedChar();
	bool							ReadUnsignedChar( unsigned char& outValue );
	char							ReadChar();
	bool							ReadChar( char& outValue );
	unsigned short					ReadUnsignedShort();
	bool							ReadUnsignedShort( unsigned short& outValue );
	short							ReadShort();
	bool							ReadShort( short& outValue );
	unsigned int					ReadUnsignedInt();
	bool							ReadUnsignedInt( unsigned int& outValue );
	int								ReadInt();
	bool							ReadInt( int& outValue );
	float							ReadFloat();
	bool							ReadFloat( float& outValue );
	double							ReadDouble();
	bool							ReadDouble( double& outValue );
	long							ReadLong();
	bool							ReadLong( long& outValue );
	bool							ReadVec2f( Vec2f& outValue );
	bool							ReadVec3f( Vec3f& outValue );
	bool							ReadRgba( Rgba& outValue );

	void							WriteUnsignedChar( unsigned char value );
	void							WriteChar( char value );
	void							WriteString( const char* string );
	void							WriteUnsignedInt( unsigned int value );
	void							WriteInt( int value );
	void							WriteFloat( float value );
	void							WriteDouble( double value );
	void							WriteLong( long value );
	void							WriteVec2f( const Vec2f& value );
	void							WriteVec3f( const Vec3f& value );
	void							WriteRgba( const Rgba& value );

private:
	std::vector< unsigned char >*	m_outBuffer;
	bool							m_isBinary;
	bool							m_isWriting;
	bool							m_isReverseEndian;
	const unsigned char*			m_readScan;
	const unsigned char*			m_readEnd;
	size_t							m_bufferSize;

};

#endif