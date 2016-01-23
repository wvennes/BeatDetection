//---------------------------------------------------------------------------
// BufferParser.cpp
//---------------------------------------------------------------------------

#include <cctype>
#include "Engine/Core/BufferParser.hpp"
#define STATIC


//---------------------------------------------------------------------------
BufferParser::BufferParser( const unsigned char* scanBegin, const size_t bufferSize, const bool isBinary )
	: m_outBuffer( nullptr )
	, m_readScan( scanBegin )
	, m_bufferSize( bufferSize )
	, m_readEnd( scanBegin + bufferSize )
	, m_isBinary( isBinary )
	, m_isReverseEndian( false )
	, m_isWriting( false )
{

}


//---------------------------------------------------------------------------
BufferParser::BufferParser( std::vector< unsigned char >& outBuffer, const bool isBinary )
	: m_outBuffer( &outBuffer )
	, m_readScan( outBuffer.data() )
	, m_bufferSize( outBuffer.capacity() )
	, m_readEnd( outBuffer.data() + outBuffer.capacity() )
	, m_isBinary( isBinary )
	, m_isReverseEndian( false )
	, m_isWriting( true )
{

}


//---------------------------------------------------------------------------
BufferParser::~BufferParser()
{
	//if ( m_outBuffer ) delete m_outBuffer;
}


//---------------------------------------------------------------------------
bool BufferParser::IsScanBeyondBuffer() const
{
	//return false;
	return m_readScan >= m_readEnd;
}


//---------------------------------------------------------------------------
void BufferParser::AdvancePastString()
{
	while ( *m_readScan != '\0' )
	{
		++ m_readScan;
	}
	++ m_readScan;
}


//---------------------------------------------------------------------------
STATIC void BufferParser::AdvanceToWhiteSpace( const char*& scan )
{
	while ( !isspace( *scan ) )
	{
		++ scan;
	}
}


//---------------------------------------------------------------------------
STATIC void BufferParser::AdvanceToNextLine( const char*& scan )
{
	while ( ( *scan ) != '\n' && ( *scan ) != '\0' )
	{
		++ scan;
	}
}


//---------------------------------------------------------------------------
STATIC void BufferParser::AdvanceToNextNonWhitespace( const char*& scan )
{
	++ scan;
	while ( isspace( *scan ) )
	{
		++ scan;
	}
}


//---------------------------------------------------------------------------
STATIC void BufferParser::AdvanceToToken( const char*& scan, const char token )
{
	++ scan;
	while ( ( *scan ) != token )
	{
		++ scan;
	}
}


//---------------------------------------------------------------------------
STATIC bool BufferParser::IsEqualToString( const char* begin, const char* end, const char* comparison )
{
	bool result = true;
	int count = 0;
	for ( ; begin != end; ++ begin )
	{
		if ( begin[ 0 ] != comparison[ count ] )
		{
			result = false;
			break;
		}

		++ count;
	}

	return result;
}


//---------------------------------------------------------------------------
STATIC void BufferParser::CopyStringToBuffer( const char* begin, const char* end, char* const buffer )
{
	int currentIndex = 0;
	while ( begin != end )
	{
		buffer[ currentIndex ] = begin[ 0 ];
		++ begin;
		++ currentIndex;
	}
	buffer[ currentIndex ] = '\0'; // null terminate
}


//---------------------------------------------------------------------------
STATIC bool BufferParser::IsTokenInString( const char* begin, const char* end, const char token )
{
	while ( begin != end )
	{
		if ( *begin == token ) return true;
		++ begin;
	}

	return false;
}


//---------------------------------------------------------------------------
STATIC int BufferParser::GetNumberOfStringsInLine( const char* scanBegin )
{
	int count = 0;
	const char* begin = scanBegin;

	if ( *begin == ' ' ) AdvanceToNextNonWhitespace( begin ); // don't count blank space towards the count

	const char* end = begin;
	AdvanceToNextLine( end );

	while ( begin != end )
	{
		if ( *begin == ' ' )
		{
			++ count;

			if ( isspace( *( begin + 1 ) ) )
			{
				-- count;
			}
		}

		++ begin;
	}
	if ( *begin != ' ' ) ++ count; // count the last string in the line
	return count;
}


//---------------------------------------------------------------------------
const unsigned char* const BufferParser::GetReadScan()
{
	return m_readScan;
}


//---------------------------------------------------------------------------
unsigned char BufferParser::ReadUnsignedChar()
{
	if ( IsScanBeyondBuffer() ) return false;

	unsigned char result = *m_readScan;
	m_readScan += sizeof( unsigned char );
	return result;
}


//---------------------------------------------------------------------------
bool BufferParser::ReadUnsignedChar( unsigned char& outValue )
{
	if ( IsScanBeyondBuffer() ) return false;

	outValue = *m_readScan;
	m_readScan += sizeof( unsigned char );

	return true;
}


//---------------------------------------------------------------------------
char BufferParser::ReadChar()
{
	if ( IsScanBeyondBuffer() ) return false;

	char result = *m_readScan;
	m_readScan += sizeof( char );
	return result;
}


//---------------------------------------------------------------------------
bool BufferParser::ReadChar( char& outValue )
{
	if ( IsScanBeyondBuffer() ) return false;

	outValue = *m_readScan;
	m_readScan += sizeof( char );

	return true;
}


//---------------------------------------------------------------------------
unsigned short BufferParser::ReadUnsignedShort()
{
	if ( IsScanBeyondBuffer() ) return false;

	unsigned short result = *( ( unsigned short* ) m_readScan );
	m_readScan += sizeof( unsigned short );
	return result;
}


//---------------------------------------------------------------------------
bool BufferParser::ReadUnsignedShort( unsigned short& outValue )
{
	if ( IsScanBeyondBuffer() ) return false;

	outValue = *( ( unsigned short* ) m_readScan );
	m_readScan += sizeof( unsigned short );

	return true;
}


//---------------------------------------------------------------------------
short BufferParser::ReadShort()
{
	if ( IsScanBeyondBuffer() ) return false;

	short result = *( ( short* ) m_readScan );
	m_readScan += sizeof( short );
	return result;
}


//---------------------------------------------------------------------------
bool BufferParser::ReadShort( short& outValue )
{
	if ( IsScanBeyondBuffer() ) return false;

	outValue = *( ( short* ) m_readScan );
	m_readScan += sizeof( short );

	return true;
}


//---------------------------------------------------------------------------
unsigned int BufferParser::ReadUnsignedInt()
{
	if ( IsScanBeyondBuffer() ) return false;

	unsigned int result = *( ( unsigned int* ) m_readScan );
	m_readScan += sizeof( unsigned int );
	return result;
}


//---------------------------------------------------------------------------
bool BufferParser::ReadUnsignedInt( unsigned int& outValue )
{
	if ( IsScanBeyondBuffer() ) return false;

	outValue = *( ( unsigned int* ) m_readScan );
	m_readScan += sizeof( unsigned int );

	return true;
}


//---------------------------------------------------------------------------
int BufferParser::ReadInt()
{
	if ( IsScanBeyondBuffer() ) return false;

	int result = *( ( int* ) m_readScan );
	m_readScan += sizeof( int );
	return result;
}


//---------------------------------------------------------------------------
bool BufferParser::ReadInt( int& outValue )
{
	if ( IsScanBeyondBuffer() ) return false;

	outValue = *( ( int* ) m_readScan );
	m_readScan += sizeof( int );

	return true;
}


//---------------------------------------------------------------------------
float BufferParser::ReadFloat()
{
	if ( IsScanBeyondBuffer() ) return false;

	float result = *( ( float* ) m_readScan );
	m_readScan += sizeof( float );
	return result;
}


//---------------------------------------------------------------------------
bool BufferParser::ReadFloat( float& outValue )
{
	if ( IsScanBeyondBuffer() ) return false;

	outValue = *( ( float* ) m_readScan );
	m_readScan += sizeof( float );

	return true;
}

//---------------------------------------------------------------------------
double BufferParser::ReadDouble()
{
	if ( IsScanBeyondBuffer() ) return false;

	double result = *( ( double* ) m_readScan );
	m_readScan += sizeof( double );
	return result;
}


//---------------------------------------------------------------------------
bool BufferParser::ReadDouble( double& outValue )
{
	if ( IsScanBeyondBuffer() ) return false;

	outValue = *( ( double* ) m_readScan );
	m_readScan += sizeof( double );

	return true;
}

//---------------------------------------------------------------------------
long BufferParser::ReadLong()
{
	long result = *( ( long* ) m_readScan );
	m_readScan += sizeof( long );
	return result;
}


//---------------------------------------------------------------------------
bool BufferParser::ReadLong( long& outValue )
{
	if ( IsScanBeyondBuffer() ) return false;

	outValue = *( ( long* ) m_readScan );
	m_readScan += sizeof( long );

	return true;
}


//---------------------------------------------------------------------------
bool BufferParser::ReadVec2f( Vec2f& outValue )
{
	bool didRead;

	didRead = ReadFloat( outValue.x );
	didRead = ReadFloat( outValue.y );

	return didRead;
}


//---------------------------------------------------------------------------
bool BufferParser::ReadVec3f( Vec3f& outValue )
{
	bool didRead;

	didRead = ReadFloat( outValue.x );
	didRead = ReadFloat( outValue.y );
	didRead = ReadFloat( outValue.z );

	return didRead;
}


//---------------------------------------------------------------------------
bool BufferParser::ReadRgba( Rgba& outValue )
{
	bool didRead;

	didRead = ReadUnsignedChar( outValue.r );
	didRead = ReadUnsignedChar( outValue.g );
	didRead = ReadUnsignedChar( outValue.b );
	didRead = ReadUnsignedChar( outValue.a );

	return didRead;
}


//---------------------------------------------------------------------------
void BufferParser::WriteUnsignedChar( unsigned char value )
{
	if ( m_outBuffer == nullptr ) return;

	for ( size_t index = 0; index < sizeof( value ); ++ index )
	{
		unsigned char byteValue = *( ( ( unsigned char* ) &value ) + index );
		m_outBuffer->push_back( byteValue );
	}
}


//---------------------------------------------------------------------------
void BufferParser::WriteChar( char value )
{
	if ( m_outBuffer == nullptr ) return;

	for ( size_t index = 0; index < sizeof( value ); ++ index )
	{
		unsigned char byteValue = *( ( ( unsigned char* ) &value ) + index );
		m_outBuffer->push_back( byteValue );
	}
}


//---------------------------------------------------------------------------
void BufferParser::WriteString( const char* string )
{
	if ( m_outBuffer == nullptr ) return;
	const char* scan = string;

	while ( *scan != '\0' )
	{
		WriteChar( *scan );

		++ scan;
	}

	WriteChar( '\0' ); // write null terminator
}


//---------------------------------------------------------------------------
void BufferParser::WriteUnsignedInt( unsigned int value )
{
	if ( m_outBuffer == nullptr ) return;

	for ( size_t index = 0; index < sizeof( value ); ++ index )
	{
		unsigned char byteValue = *( ( ( unsigned char* ) &value ) + index );
		m_outBuffer->push_back( byteValue );
	}
}


//---------------------------------------------------------------------------
void BufferParser::WriteInt( int value )
{
	if ( m_outBuffer == nullptr ) return;

	for ( size_t index = 0; index < sizeof( value ); ++ index )
	{
		m_outBuffer->push_back( *( ( ( unsigned char* ) &value ) + index ) );
	}
}


//---------------------------------------------------------------------------
void BufferParser::WriteFloat( float value )
{
	if ( m_outBuffer == nullptr ) return;

	for ( size_t index = 0; index < sizeof( value ); ++ index )
	{
		m_outBuffer->push_back( *( ( ( unsigned char* ) &value ) + index ) );
	}
}


//---------------------------------------------------------------------------
void BufferParser::WriteDouble( double value )
{
	if ( m_outBuffer == nullptr ) return;

	for ( size_t index = 0; index < sizeof( value ); ++ index )
	{
		m_outBuffer->push_back( *( ( ( unsigned char* ) &value ) + index ) );
	}
}


//---------------------------------------------------------------------------
void BufferParser::WriteLong( long value )
{
	if ( m_outBuffer == nullptr ) return;

	for ( size_t index = 0; index < sizeof( value ); ++ index )
	{
		m_outBuffer->push_back( *( ( ( unsigned char* ) &value ) + index ) );
	}
}


//---------------------------------------------------------------------------
void BufferParser::WriteVec2f( const Vec2f& value )
{
	if ( m_outBuffer == nullptr ) return;

	WriteFloat( value.x );
	WriteFloat( value.y );
}


//---------------------------------------------------------------------------
void BufferParser::WriteVec3f( const Vec3f& value )
{
	if ( m_outBuffer == nullptr ) return;

	WriteFloat( value.x );
	WriteFloat( value.y );
	WriteFloat( value.z );
}


//---------------------------------------------------------------------------
void BufferParser::WriteRgba( const Rgba& value )
{
	if ( m_outBuffer == nullptr ) return;

	WriteUnsignedChar( value.r );
	WriteUnsignedChar( value.g );
	WriteUnsignedChar( value.b );
	WriteUnsignedChar( value.a );
}