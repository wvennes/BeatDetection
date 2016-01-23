//---------------------------------------------------------------------------
// Mesh.cpp
//---------------------------------------------------------------------------

#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/Rendering/C23FileHandler.hpp"
#include "Engine/Rendering/OBJFileHandler.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Mesh.hpp"


//---------------------------------------------------------------------------
Mesh::Mesh()
	: m_vboID ( 0 )
	, m_iboID ( 0 )
	, m_vertexCount( 0 )
	, m_indexCount( 0 )
{

}


//---------------------------------------------------------------------------
Mesh::~Mesh()
{
	if ( m_vboID != 0 ) g_theRenderer->VennDeleteBuffers( 1, &m_vboID );
	if ( m_iboID != 0 ) g_theRenderer->VennDeleteBuffers( 1, &m_iboID );
}


//---------------------------------------------------------------------------
void Mesh::LoadDataAndMakeBufferObjectsFromObj( const char* meshFilePath )
{
	std::vector< Vertex3D > vertexes;
	std::vector< unsigned int > indexes;
	vertexes.reserve( 20000 );
	indexes.reserve( 20000 );

	OBJFileHandler objHandler;
	objHandler.ReadFromObjAndStoreVertexesAndIndexes( meshFilePath, vertexes, indexes );

	// construct VBO
	m_vertexCount = vertexes.size();
	m_indexCount = indexes.size();

	if ( m_vboID == 0 )
	{
		g_theRenderer->VennGenBuffers( 1, &m_vboID );
	}
	if ( m_iboID == 0 )
	{
		g_theRenderer->VennGenBuffers( 1, &m_iboID );
	}

	// bind VBO
	g_theRenderer->VennBindBuffer( V_ARRAY_BUFFER, m_vboID );
	g_theRenderer->VennBufferData( V_ARRAY_BUFFER, sizeof( Vertex3D ) * m_vertexCount, vertexes.data(), V_STATIC_DRAW );
	g_theRenderer->VennBindBuffer( V_ARRAY_BUFFER, 0 );

	// bind IBO
	g_theRenderer->VennBindBuffer( V_ELEMENT_ARRAY_BUFFER, m_iboID );
	g_theRenderer->VennBufferData( V_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * m_indexCount, indexes.data(), V_STATIC_DRAW );
	g_theRenderer->VennBindBuffer( V_ELEMENT_ARRAY_BUFFER, 0 );

// 	std::string filePath( meshFilePath );
// 	filePath = filePath.substr( 0, filePath.find_last_of( '.' ) ) + ".C23";
// 	C23FileHandler c23Handler;
// 	c23Handler.WriteToFile( filePath.c_str(), vertexes, indexes );

	vertexes.clear();
	indexes.clear();
}


//---------------------------------------------------------------------------
void Mesh::LoadDataAndMakeBufferObjectsFromC23( const char* meshFilePath )
{
	std::vector< Vertex3D > vertexes;
	std::vector< unsigned int > indexes;

	C23FileHandler handler;
	handler.ReadFromFileAndStoreVertexesAndIndexes( meshFilePath, vertexes, indexes );

	// construct VBO
	m_vertexCount = vertexes.size();
	m_indexCount = indexes.size();

	if ( m_vboID == 0 )
	{
		g_theRenderer->VennGenBuffers( 1, &m_vboID );
	}
	if ( m_iboID == 0 )
	{
		g_theRenderer->VennGenBuffers( 1, &m_iboID );
	}

	// bind VBO
	g_theRenderer->VennBindBuffer( V_ARRAY_BUFFER, m_vboID );
	g_theRenderer->VennBufferData( V_ARRAY_BUFFER, sizeof( Vertex3D ) * m_vertexCount, vertexes.data(), V_STATIC_DRAW );
	g_theRenderer->VennBindBuffer( V_ARRAY_BUFFER, 0 );

	// bind IBO
	g_theRenderer->VennBindBuffer( V_ELEMENT_ARRAY_BUFFER, m_iboID );
	g_theRenderer->VennBufferData( V_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * m_indexCount, indexes.data(), V_STATIC_DRAW );
	g_theRenderer->VennBindBuffer( V_ELEMENT_ARRAY_BUFFER, 0 );

	vertexes.clear();
	indexes.clear();
}


//---------------------------------------------------------------------------
void Mesh::Render( const unsigned int shaderProgramID )
{
	//RenderVBOWithShaderAndNormals( m_vboID, m_vertexCount, shaderProgramID, V_TRIANGLES );
	//RenderVBO( m_vboID, m_vertexCount, V_TRIANGLES );
	//RenderVertexArray( m_vertexes.data(), m_vertexes.size(), V_TRIANGLES );
	//RenderElements( m_vertexes.data(), m_vertexes.size(), m_indexes.data(), m_indexes.size(), V_TRIANGLES );
	RenderElementsWithVBOAndIBO( m_vboID, m_iboID, m_indexCount, shaderProgramID, V_TRIANGLES );
}