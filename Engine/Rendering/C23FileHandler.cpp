//---------------------------------------------------------------------------
// C23FileHandler.cpp
//---------------------------------------------------------------------------

#include "Engine/Rendering/C23FileHandler.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Core/BufferParser.hpp"
#include "Engine/Core/EngineCommon.hpp"


//---------------------------------------------------------------------------
C23FileHandler::C23FileHandler()
	: m_bufferParser( nullptr )
{

}


//---------------------------------------------------------------------------
C23FileHandler::~C23FileHandler()
{
	if ( m_bufferParser != nullptr ) delete m_bufferParser;
}


// unsigned char* g_buffer = nullptr;
// std::vector< Vertex3D > g_vertexes;
// std::vector< unsigned int > g_indexes;
// bool g_hasBufferFilled = false;
// bool g_hasVertexesFilled = false;
// bool g_hasIndexesFilled = false;
// bool g_compareVertexes = false;


//---------------------------------------------------------------------------
void C23FileHandler::ReadFromFileAndStoreVertexesAndIndexes( const char* filePath, std::vector< Vertex3D >& vertexes, std::vector< unsigned int >& indexes )
{
	unsigned char* buffer = nullptr;
	FILE* file = nullptr;
	fopen_s( &file, filePath, "rb" );

	if ( file == nullptr ) return;

	// load to buffer
	fseek( file, 0, SEEK_END );
	size_t fileBufferSize = ftell( file );
	rewind( file );
	//size_t fileBufferPadding = 5;
	//fileBufferSize += fileBufferPadding;

// 	if ( !g_hasBufferFilled )
// 	{
// 		g_buffer = new unsigned char[ fileBufferSize ];
// 
// 		fread( g_buffer, sizeof( unsigned char ), fileBufferSize, file );
// 		fclose( file );
// 		g_hasBufferFilled = true;
// 		buffer = g_buffer;
// 	}
// 	else
	{
		buffer = new unsigned char[ fileBufferSize ];

		fread( buffer, sizeof( unsigned char ), fileBufferSize, file );
		fclose( file );

// 		for ( size_t index = 0; index < fileBufferSize; ++ index )
// 		{
// 			//DebuggerPrintf( "index = %i, g_buffer = %i, buffer = %i\n", index, g_buffer[ index ], buffer[ index ] );
// 			if ( g_buffer[ index ] != buffer[ index ] )
// 			{
// 				DebuggerPrintf( "ERROR: index = %i, g_buffer = %i, buffer = %i\n", index, g_buffer[ index ], buffer[ index ] );
// 			}
// 		}
	}
	
	// fill padding with extra null terminators to ensure no overflow
// 	for ( int count = 0; count < fileBufferPadding; ++ count )
// 	{
// 		buffer[ fileBufferSize - count - 1 ] = '\0';
// 	}

	m_bufferParser = new BufferParser( buffer, fileBufferSize, true );

	// read FourCC
	std::string fourCC;
	fourCC += m_bufferParser->ReadUnsignedChar();
	fourCC += m_bufferParser->ReadUnsignedChar();
	fourCC += m_bufferParser->ReadUnsignedChar();
	fourCC += m_bufferParser->ReadUnsignedChar();

	if ( fourCC != "GC23" ) // if the fourCC doesn't match, fail
	{
		delete buffer;
		delete m_bufferParser;
		m_bufferParser = nullptr;
		return;
	}

	// read subtype number
	unsigned char subType = 0;
	m_bufferParser->ReadUnsignedChar( subType );

	// read version number
	unsigned char versionNumber = 0;
	m_bufferParser->ReadUnsignedChar( versionNumber );

	// skip comment
	m_bufferParser->AdvancePastString();

	// only read indexes for subType 2 (indexed models)
	if ( subType == 2 )
	{
		// read index size
		unsigned int numIndexes = 0;
		m_bufferParser->ReadUnsignedInt( numIndexes );

		// read indexes
		indexes.reserve( numIndexes );
		for ( unsigned int count = 0; count < numIndexes; ++ count )
		{
			unsigned int value = m_bufferParser->ReadUnsignedInt();
			indexes.push_back( value );
			//if ( !g_hasIndexesFilled ) g_indexes.push_back( value );
		}
		//g_hasIndexesFilled = true;
	}

	// read vertex size
	unsigned int numVertexes = 0;
	m_bufferParser->ReadUnsignedInt( numVertexes );

	// read vertexes
	Vertex3D currentVertex;
	vertexes.reserve( numVertexes );

	for ( unsigned int count = 0; count < numVertexes; ++ count )
	{
		ReadVertex( currentVertex );
		vertexes.push_back( currentVertex );
		//if ( !g_hasVertexesFilled ) g_vertexes.push_back( currentVertex );
	}
	//g_hasVertexesFilled = true;

	// fill in indexes with default values if not subtype 2
	if ( indexes.empty() )
	{
		indexes.reserve( numVertexes );
		for ( unsigned int count = 0; count < numVertexes; ++ count )
		{
			indexes.push_back( ( unsigned int ) indexes.size() );
		}
	}

	// construct VBO
	//m_vertexCount = vertexes.size();

// 	if ( vboID == 0 )
// 	{
// 		g_theRenderer->VennGenBuffers( 1, &vboID );
// 	}
// 
// 	g_theRenderer->VennBindBuffer( V_ARRAY_BUFFER, vboID );
// 	g_theRenderer->VennBufferData( V_ARRAY_BUFFER, sizeof( Vertex3D ) * numVertexes, m_bufferParser->GetReadScan(), V_STATIC_DRAW );
// 	g_theRenderer->VennBindBuffer( V_ARRAY_BUFFER, 0 );

// 	if ( g_compareVertexes )
// 	{
// 		DebuggerPrintf( "g_vertex size = %i, g_index size = %i\n", g_vertexes.size(), g_indexes.size() );
// 		DebuggerPrintf( "vertex size = %i, index size = %i\n", vertexes.size(), indexes.size() );
// 
// 		for ( unsigned int index = 0; index < g_vertexes.size(); ++ index )
// 		{
// 			if ( g_vertexes[ index ].m_position != vertexes[ index ].m_position )
// 			{
// 				DebuggerPrintf( "ERROR VERTEX: index = %i\n", index );
// 				DebuggerPrintf( "g_vertex.x = %f, vertex.x = %f\n", g_vertexes[ index ].m_position.x, vertexes[ index ].m_position.x  );
// 				DebuggerPrintf( "g_vertex.y = %f, vertex.y = %f\n", g_vertexes[ index ].m_position.y, vertexes[ index ].m_position.y  );
// 				DebuggerPrintf( "g_vertex.z = %f, vertex.z = %f\n", g_vertexes[ index ].m_position.z, vertexes[ index ].m_position.z  );
// 			}
// 			if ( g_vertexes[ index ].m_normal != vertexes[ index ].m_normal )
// 			{
// 				DebuggerPrintf( "ERROR NORMAL: index = %i\n", index );
// 				DebuggerPrintf( "g_vertex.x = %f, vertex.x = %f\n", g_vertexes[ index ].m_normal.x, vertexes[ index ].m_normal.x  );
// 				DebuggerPrintf( "g_vertex.y = %f, vertex.y = %f\n", g_vertexes[ index ].m_normal.y, vertexes[ index ].m_normal.y  );
// 				DebuggerPrintf( "g_vertex.z = %f, vertex.z = %f\n", g_vertexes[ index ].m_normal.z, vertexes[ index ].m_normal.z  );
// 			}
// 			if ( g_indexes[ index ] != indexes[ index ] )
// 			{
// 				DebuggerPrintf( "ERROR INDEX: index = %i, g_vertex.x = %f, vertex.x = %f\n", index, g_indexes[ index ], indexes[ index ] );
// 			}
// 		}
// 	}
// 	g_compareVertexes = true;
	
	if ( buffer ) delete buffer;
	if ( m_bufferParser ) delete m_bufferParser;
	m_bufferParser = nullptr;
}


//---------------------------------------------------------------------------
void C23FileHandler::WriteToFile( const char* filePath, std::vector< Vertex3D >& vertexes, std::vector< unsigned int >& indexes )
{
	std::vector< unsigned char > buffer;
	size_t sizeGuess = ( sizeof( Vertex3D ) * vertexes.size() ) + 1024;
	buffer.reserve( sizeGuess );

	m_bufferParser = new BufferParser( buffer, true );

	// write FourCC code
	m_bufferParser->WriteChar( 'G' );
	m_bufferParser->WriteChar( 'C' );
	m_bufferParser->WriteChar( '2' );
	m_bufferParser->WriteChar( '3' );

	// subtype
	m_bufferParser->WriteUnsignedChar( 2 );

	// version number
	m_bufferParser->WriteUnsignedChar( 1 );

	// comment text
	m_bufferParser->WriteString( "Generated comment." );

	// number of indexes
	m_bufferParser->WriteUnsignedInt( ( unsigned int ) indexes.size() );

	// write index data
	for ( unsigned int index = 0; index < ( unsigned int ) indexes.size(); ++ index )
	{
		m_bufferParser->WriteUnsignedInt( indexes[ index ] );
	}

	// number of vertexes
	m_bufferParser->WriteUnsignedInt( ( unsigned int ) vertexes.size() );

	// write vertex data
	for ( unsigned int index = 0; index < ( unsigned int ) vertexes.size(); ++ index )
	{
		WriteVertex( vertexes[ index ] );
	}

	// write to file
	FILE* file = nullptr;
	fopen_s( &file, filePath, "wb" );

	if ( file == nullptr ) return;

	fwrite( &buffer.data()[ 0 ], sizeof( unsigned char ), buffer.size(), file );

	fclose( file );

	delete m_bufferParser;
	m_bufferParser = nullptr;
}


//---------------------------------------------------------------------------
void C23FileHandler::ReadVertex( Vertex3D& vertex )
{
	m_bufferParser->ReadVec3f( vertex.m_position );
	m_bufferParser->ReadRgba( vertex.m_color );
	m_bufferParser->ReadVec2f( vertex.m_textureCoords );
	m_bufferParser->ReadVec3f( vertex.m_normal );
	m_bufferParser->ReadVec3f( vertex.m_tangent );
	m_bufferParser->ReadVec3f( vertex.m_bitangent );
}


//---------------------------------------------------------------------------
void C23FileHandler::WriteVertex( const Vertex3D& vertex )
{
	m_bufferParser->WriteVec3f( vertex.m_position );
	m_bufferParser->WriteRgba( vertex.m_color );
	m_bufferParser->WriteVec2f( vertex.m_textureCoords );
	m_bufferParser->WriteVec3f( vertex.m_normal );
	m_bufferParser->WriteVec3f( vertex.m_tangent );
	m_bufferParser->WriteVec3f( vertex.m_bitangent );
}