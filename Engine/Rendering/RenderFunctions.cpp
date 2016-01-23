//---------------------------------------------------------------------------
// RenderFunctions.cpp
// * OpenGL convenience functions
// NOTE: Assumes textures are bound and transforms are done beforehand
//---------------------------------------------------------------------------
#include "Engine/Rendering/Vertex3D.hpp"
#include "Engine/Math/Colors.hpp"
#include <string>
#include <iostream>
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/Core/EngineCommon.hpp"


//---------------------------------------------------------------------------
void DrawQuad()
{
	g_theRenderer->VennBegin( V_QUADS );
	{
		g_theRenderer->VennVertex2f( 0.f, 1.f );
		g_theRenderer->VennVertex2f( 0.f, 0.f );
		g_theRenderer->VennVertex2f( 1.f, 0.f );
		g_theRenderer->VennVertex2f( 1.f, 1.f );
	}
	g_theRenderer->VennEnd();
}


//---------------------------------------------------------------------------
void DrawQuadColor( const Vec3f& color )
{
	g_theRenderer->VennBegin( V_QUADS );
	{
		g_theRenderer->VennColor3f( color.x, color.y, color.z );
		g_theRenderer->VennVertex2f( 0.f, 1.f );
		g_theRenderer->VennColor3f( color.x, color.y, color.z );
		g_theRenderer->VennVertex2f( 0.f, 0.f );
		g_theRenderer->VennColor3f( color.x, color.y, color.z );
		g_theRenderer->VennVertex2f( 1.f, 0.f );
		g_theRenderer->VennColor3f( color.x, color.y, color.z );
		g_theRenderer->VennVertex2f( 1.f, 1.f );
	}
	g_theRenderer->VennEnd();
}


//---------------------------------------------------------------------------
void DrawQuadWithShader( const unsigned int shaderProgramID )
{
	Vertex3D vertices[ 4 ];

	vertices[ 0 ].m_position = Vec3f( 0.f, 0.f, 0.f );
	vertices[ 1 ].m_position = Vec3f( 0.f, 1.f, 0.f );
	vertices[ 2 ].m_position = Vec3f( 1.f, 1.f, 0.f );
	vertices[ 3 ].m_position = Vec3f( 1.f, 0.f, 0.f );

	vertices[ 0 ].m_textureCoords = Vec2f( 0.f, 1.f );
	vertices[ 1 ].m_textureCoords = Vec2f( 0.f, 0.f );
	vertices[ 2 ].m_textureCoords = Vec2f( 1.f, 0.f );
	vertices[ 3 ].m_textureCoords = Vec2f( 1.f, 1.f );

	RenderVertexArrayWithShader( vertices, 4, shaderProgramID, V_QUADS );
}


//---------------------------------------------------------------------------
void DrawLineQuad()
{
	g_theRenderer->VennBegin( V_LINE_LOOP );
	{
		g_theRenderer->VennVertex2f( 0.f, 1.f );
		g_theRenderer->VennVertex2f( 0.f, 0.f );
		g_theRenderer->VennVertex2f( 1.f, 0.f );
		g_theRenderer->VennVertex2f( 1.f, 1.f );
	}
	g_theRenderer->VennEnd();
}


//---------------------------------------------------------------------------
void DrawQuadAboutCenter()
{
	g_theRenderer->VennBegin( V_QUADS );
	{
		g_theRenderer->VennVertex2f( -1.f, 1.f );
		g_theRenderer->VennVertex2f( -1.f, -1.f );
		g_theRenderer->VennVertex2f( 1.f, -1.f );
		g_theRenderer->VennVertex2f( 1.f, 1.f );
	}
	g_theRenderer->VennEnd();
}


//---------------------------------------------------------------------------
void DrawTexturedQuad()
{
	g_theRenderer->VennBegin( V_QUADS );
	{
		g_theRenderer->VennTexCoord2f( 0.f, 0.f );
		g_theRenderer->VennVertex2f( 0.f, 1.f );

		g_theRenderer->VennTexCoord2f( 0.f, 1.f );
		g_theRenderer->VennVertex2f( 0.f, 0.f );

		g_theRenderer->VennTexCoord2f( 1.f, 1.f );
		g_theRenderer->VennVertex2f( 1.f, 0.f );

		g_theRenderer->VennTexCoord2f( 1.f, 0.f );
		g_theRenderer->VennVertex2f( 1.f, 1.f );
	}
	g_theRenderer->VennEnd();
}


//---------------------------------------------------------------------------
void DrawTexturedQuadAboutCenter()
{
	g_theRenderer->VennBegin( V_QUADS );
	{
		g_theRenderer->VennTexCoord2f( 0.f, 0.f );
		g_theRenderer->VennVertex2f( -1.f, 1.f );

		g_theRenderer->VennTexCoord2f( 0.f, 1.f );
		g_theRenderer->VennVertex2f( -1.f, -1.f );

		g_theRenderer->VennTexCoord2f( 1.f, 1.f );
		g_theRenderer->VennVertex2f( 1.f, -1.f );

		g_theRenderer->VennTexCoord2f( 1.f, 0.f );
		g_theRenderer->VennVertex2f( 1.f, 1.f );
	}
	g_theRenderer->VennEnd();
}


//---------------------------------------------------------------------------
void DrawTexturedQuadFromSpriteSheet( const Vec2f& minTexCoords, const Vec2f& maxTexCoords )
{
	g_theRenderer->VennBegin( V_QUADS );
	{
		g_theRenderer->VennTexCoord2f( minTexCoords.x, minTexCoords.y );
		g_theRenderer->VennVertex2f( 0.f, 1.f );

		g_theRenderer->VennTexCoord2f( minTexCoords.x, maxTexCoords.y );
		g_theRenderer->VennVertex2f( 0.f, 0.f );

		g_theRenderer->VennTexCoord2f( maxTexCoords.x, maxTexCoords.y );
		g_theRenderer->VennVertex2f( 1.f, 0.f );

		g_theRenderer->VennTexCoord2f( maxTexCoords.x, minTexCoords.y );
		g_theRenderer->VennVertex2f( 1.f, 1.f );
	}
	g_theRenderer->VennEnd();
}


//---------------------------------------------------------------------------
void DrawTexturedQuadFromSpriteSheetAboutCenter( const Vec2f& minTexCoords, const Vec2f& maxTexCoords )
{
	g_theRenderer->VennBegin( V_QUADS );
	{
		g_theRenderer->VennTexCoord2f( minTexCoords.x, minTexCoords.y );
		g_theRenderer->VennVertex2f( -1.f, 1.f );

		g_theRenderer->VennTexCoord2f( minTexCoords.x, maxTexCoords.y );
		g_theRenderer->VennVertex2f( -1.f, -1.f );

		g_theRenderer->VennTexCoord2f( maxTexCoords.x, maxTexCoords.y );
		g_theRenderer->VennVertex2f( 1.f, -1.f );

		g_theRenderer->VennTexCoord2f( maxTexCoords.x, minTexCoords.y );
		g_theRenderer->VennVertex2f( 1.f, 1.f );
	}
	g_theRenderer->VennEnd();
}


//---------------------------------------------------------------------------
void DrawAxisAtCenter()
{
// 	g_theRenderer->VennBegin( V_LINES );
// 	{
// 		g_theRenderer->VennColor3f( 1.f, 0.f, 0.f );
// 		g_theRenderer->VennVertex3f( 0.f, 0.f, 0.f );
// 		g_theRenderer->VennVertex3f( 0.f, 0.f, 0.f );
// 
// 		g_theRenderer->VennColor3f( 0.f, 1.f, 0.f );
// 		g_theRenderer->VennVertex3f( 0.f, 0.f, 0.f );
// 		g_theRenderer->VennVertex3f( 0.f, 0.f, 0.f );
// 
// 		g_theRenderer->VennColor3f( 0.f, 0.f, 1.f );
// 		g_theRenderer->VennVertex3f( 0.f, 0.f, 0.f );
// 		g_theRenderer->VennVertex3f( 0.f, 0.f, 0.f );
// 	}
// 	g_theRenderer->VennEnd();

	Vertex3D vertices[ 6 ];

	const float axisLength = 1.f;
	const float axisOffset = 0.f;

	vertices[ 0 ].m_position = Vec3f( axisOffset, 0.f, 0.f );
	vertices[ 0 ].m_color = Colors::RED;
	vertices[ 1 ].m_position = Vec3f( axisLength, 0.f, 0.f );
	vertices[ 1 ].m_color = Colors::RED;

	vertices[ 2 ].m_position = Vec3f( 0.f, axisOffset, 0.f );
	vertices[ 2 ].m_color = Colors::BLUE;
	vertices[ 3 ].m_position = Vec3f( 0.f, axisLength, 0.f );
	vertices[ 3 ].m_color = Colors::BLUE;

	vertices[ 4 ].m_position = Vec3f( 0.f, 0.f, axisOffset );
	vertices[ 4 ].m_color = Colors::GREEN;
	vertices[ 5 ].m_position = Vec3f( 0.f, 0.f, axisLength );
	vertices[ 5 ].m_color = Colors::GREEN;

	RenderVertexArray( vertices, 6, V_LINES );
}


//---------------------------------------------------------------------------
void DrawAxisAtCenter( unsigned int shaderProgramID )
{
	Vertex3D vertices[ 6 ];

	const float axisLength = 1.f;
	const float axisOffset = 0.f;

	vertices[ 0 ].m_position = Vec3f( axisOffset, 0.f, 0.f );
	vertices[ 0 ].m_color = Colors::RED;
	vertices[ 1 ].m_position = Vec3f( axisLength, 0.f, 0.f );
	vertices[ 1 ].m_color = Colors::RED;

	vertices[ 2 ].m_position = Vec3f( 0.f, axisOffset, 0.f );
	vertices[ 2 ].m_color = Colors::BLUE;
	vertices[ 3 ].m_position = Vec3f( 0.f, axisLength, 0.f );
	vertices[ 3 ].m_color = Colors::BLUE;

	vertices[ 4 ].m_position = Vec3f( 0.f, 0.f, axisOffset );
	vertices[ 4 ].m_color = Colors::GREEN;
	vertices[ 5 ].m_position = Vec3f( 0.f, 0.f, axisLength );
	vertices[ 5 ].m_color = Colors::GREEN;

	RenderVertexArrayWithShader( vertices, 6, shaderProgramID, V_LINES );
}

//---------------------------------------------------------------------------
void DrawTexturedCube( const Vec2f& minTexCoords, const Vec2f& maxTexCoords )
{
	g_theRenderer->VennBegin( V_QUADS );
	{
		g_theRenderer->VennColor3f( 1.f, 1.f, 1.f );

		// front face
		g_theRenderer->VennTexCoord2f( minTexCoords.x, minTexCoords.y );
		g_theRenderer->VennVertex3f( 0.f, 0.f, 1.f );
		g_theRenderer->VennTexCoord2f( minTexCoords.x, maxTexCoords.y );
		g_theRenderer->VennVertex3f( 0.f, 0.f, 0.f );
		g_theRenderer->VennTexCoord2f( maxTexCoords.x, maxTexCoords.y );
		g_theRenderer->VennVertex3f( 1.f, 0.f, 0.f );
		g_theRenderer->VennTexCoord2f( maxTexCoords.x, minTexCoords.y );
		g_theRenderer->VennVertex3f( 1.f, 0.f, 1.f );

		// left face
		g_theRenderer->VennVertex3f( 1.f, 0.f, 0.f );
		g_theRenderer->VennVertex3f( 1.f, 1.f, 0.f );
		g_theRenderer->VennVertex3f( 1.f, 1.f, 1.f );
		g_theRenderer->VennVertex3f( 1.f, 0.f, 1.f );
	}
	g_theRenderer->VennEnd();
}



//---------------------------------------------------------------------------
void RenderVertexArrayWithShader( const Vertex3D* vertices, const int arraySize, const unsigned int shaderProgramID, const VennEnum mode )
{
//	glGetAttribLocation( shaderProgramID, "v_texCoords" );
	int uniformLocation = 0;

	//OutputDebugStringA(  );
	uniformLocation = g_theRenderer->VennGetUniformLocation( shaderProgramID, "u_worldToClipSpaceMatrix" );
	g_theRenderer->VennUniformMatrix4fv( uniformLocation, 1, false, g_theRenderer->GetCurrentMatrixData() );
//	m_multiTextureMaterial->RegisterDynamicUniformMatrix4( "u_worldToClipSpaceMatrix", g_theRenderer->GetCurrentMatrixData() );


	int positionLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_position" );
	int colorLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_color" );
	int texCoordLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_texCoords" );

	g_theRenderer->VennEnableVertexAttribArray( positionLocation );
	g_theRenderer->VennEnableVertexAttribArray( colorLocation );
	g_theRenderer->VennEnableVertexAttribArray( texCoordLocation );

	g_theRenderer->VennVertexAttribPointer( positionLocation,	3, V_FLOAT,			false,	sizeof( Vertex3D ), &vertices[0].m_position );
	g_theRenderer->VennVertexAttribPointer( colorLocation,		4, V_UNSIGNED_BYTE, true,	sizeof( Vertex3D ), &vertices[0].m_color );
	g_theRenderer->VennVertexAttribPointer( texCoordLocation,	2, V_FLOAT,			false,	sizeof( Vertex3D ), &vertices[0].m_textureCoords );

	g_theRenderer->VennDrawArrays( mode, 0, arraySize );

	g_theRenderer->VennDisableVertexAttribArray( positionLocation );
	g_theRenderer->VennDisableVertexAttribArray( colorLocation );
	g_theRenderer->VennDisableVertexAttribArray( texCoordLocation );
}


//---------------------------------------------------------------------------
void RenderVertexArrayWithShaderAndNormals( const Vertex3D* vertices, const int arraySize, const unsigned int shaderProgramID, const VennEnum mode )
{
	//	glGetAttribLocation( shaderProgramID, "v_texCoords" );
	int uniformLocation = g_theRenderer->VennGetUniformLocation( shaderProgramID, "u_worldToClipSpaceMatrix" );
	g_theRenderer->VennUniformMatrix4fv( uniformLocation, 1, false, ( float* ) &g_theRenderer->GetCurrentMatrixData()[0] );
	//	m_multiTextureMaterial->RegisterDynamicUniformMatrix4( "u_worldToClipSpaceMatrix", g_theRenderer->GetCurrentMatrixData() );


	int positionLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_position" );
	int colorLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_color" );
	int texCoordLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_texCoords" );
	int normalCoordLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_normal" );
	int tangentCoordLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_tangent" );
	int bitangentCoordLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_bitangent" );

	g_theRenderer->VennEnableVertexAttribArray( positionLocation );
	g_theRenderer->VennEnableVertexAttribArray( colorLocation );
	g_theRenderer->VennEnableVertexAttribArray( texCoordLocation );
	g_theRenderer->VennEnableVertexAttribArray( normalCoordLocation );
	g_theRenderer->VennEnableVertexAttribArray( tangentCoordLocation );
	g_theRenderer->VennEnableVertexAttribArray( bitangentCoordLocation );

	g_theRenderer->VennVertexAttribPointer( positionLocation,		3, V_FLOAT,			false,	sizeof( Vertex3D ), &vertices[0].m_position );
	g_theRenderer->VennVertexAttribPointer( colorLocation,			4, V_UNSIGNED_BYTE, true,	sizeof( Vertex3D ), &vertices[0].m_color );
	g_theRenderer->VennVertexAttribPointer( texCoordLocation,		2, V_FLOAT,			false,	sizeof( Vertex3D ), &vertices[0].m_textureCoords );
	g_theRenderer->VennVertexAttribPointer( normalCoordLocation,	3, V_FLOAT,			false,	sizeof( Vertex3D ), &vertices[0].m_normal );
	g_theRenderer->VennVertexAttribPointer( tangentCoordLocation,	3, V_FLOAT,			false,	sizeof( Vertex3D ), &vertices[0].m_tangent );
	g_theRenderer->VennVertexAttribPointer( bitangentCoordLocation, 3, V_FLOAT,			false,	sizeof( Vertex3D ), &vertices[0].m_bitangent );

	g_theRenderer->VennDrawArrays( mode, 0, arraySize );

	g_theRenderer->VennDisableVertexAttribArray( positionLocation );
	g_theRenderer->VennDisableVertexAttribArray( colorLocation );
	g_theRenderer->VennDisableVertexAttribArray( texCoordLocation );
	g_theRenderer->VennDisableVertexAttribArray( normalCoordLocation );
	g_theRenderer->VennDisableVertexAttribArray( tangentCoordLocation );
	g_theRenderer->VennDisableVertexAttribArray( bitangentCoordLocation );
}


//---------------------------------------------------------------------------
void RenderVertexArray( const Vertex3D* vertices, const int arraySize, const VennEnum mode )
{
	g_theRenderer->VennEnableClientState( V_VERTEX_ARRAY );
	g_theRenderer->VennEnableClientState( V_COLOR_ARRAY );
	g_theRenderer->VennEnableClientState( V_TEXTURE_COORD_ARRAY );

	g_theRenderer->VennVertexPointer(	3, V_FLOAT,			sizeof( Vertex3D ), &vertices[0].m_position );
	g_theRenderer->VennColorPointer(	4, V_UNSIGNED_BYTE, sizeof( Vertex3D ), &vertices[0].m_color );
	g_theRenderer->VennTexCoordPointer( 2, V_FLOAT,			sizeof( Vertex3D ), &vertices[0].m_textureCoords );

	g_theRenderer->VennDrawArrays( mode, 0, arraySize );

	g_theRenderer->VennDisableClientState( V_VERTEX_ARRAY );
	g_theRenderer->VennDisableClientState( V_COLOR_ARRAY );
	g_theRenderer->VennDisableClientState( V_TEXTURE_COORD_ARRAY );
}


//---------------------------------------------------------------------------
void RenderVBO( const unsigned int vboID, const int arraySize, const VennEnum mode )
{
	g_theRenderer->VennBindBuffer( V_ARRAY_BUFFER, vboID );

	g_theRenderer->VennEnableClientState( V_VERTEX_ARRAY );
	g_theRenderer->VennEnableClientState( V_COLOR_ARRAY );
	g_theRenderer->VennEnableClientState( V_TEXTURE_COORD_ARRAY );

	g_theRenderer->VennVertexPointer(	3, V_FLOAT,			sizeof( Vertex3D ), (const void*) offsetof( Vertex3D, m_position ) );	
	g_theRenderer->VennColorPointer(	4, V_UNSIGNED_BYTE, sizeof( Vertex3D ), (const void*) offsetof( Vertex3D, m_color ) );	
	g_theRenderer->VennTexCoordPointer(	2, V_FLOAT,			sizeof( Vertex3D ), (const void*) offsetof( Vertex3D, m_textureCoords ) );	

	g_theRenderer->VennDrawArrays( mode, 0, arraySize );

	g_theRenderer->VennDisableClientState( V_VERTEX_ARRAY );
	g_theRenderer->VennDisableClientState( V_COLOR_ARRAY );
	g_theRenderer->VennDisableClientState( V_TEXTURE_COORD_ARRAY );

	g_theRenderer->VennBindBuffer( V_ARRAY_BUFFER, 0 );
}


//---------------------------------------------------------------------------
void RenderVBOWithShaderAndNormals( const unsigned int vboID, const int arraySize, const unsigned int shaderProgramID, const VennEnum mode )
{
	g_theRenderer->VennBindBuffer( V_ARRAY_BUFFER, vboID );

	//	glGetAttribLocation( shaderProgramID, "v_texCoords" );
	int uniformLocation = g_theRenderer->VennGetUniformLocation( shaderProgramID, "u_worldToClipSpaceMatrix" );
	g_theRenderer->VennUniformMatrix4fv( uniformLocation, 1, false, ( float* ) &g_theRenderer->GetCurrentMatrixData()[0] );
	//	m_multiTextureMaterial->RegisterDynamicUniformMatrix4( "u_worldToClipSpaceMatrix", g_theRenderer->GetCurrentMatrixData() );


	int positionLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_position" );
	int colorLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_color" );
	int texCoordLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_texCoords" );
	int normalCoordLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_normal" );
	int tangentCoordLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_tangent" );
	int bitangentCoordLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_bitangent" );

	g_theRenderer->VennEnableVertexAttribArray( positionLocation );
	g_theRenderer->VennEnableVertexAttribArray( colorLocation );
	g_theRenderer->VennEnableVertexAttribArray( texCoordLocation );
	g_theRenderer->VennEnableVertexAttribArray( normalCoordLocation );
	g_theRenderer->VennEnableVertexAttribArray( tangentCoordLocation );
	g_theRenderer->VennEnableVertexAttribArray( bitangentCoordLocation );

	g_theRenderer->VennVertexAttribPointer( positionLocation,		3,	V_FLOAT,			false,	sizeof( Vertex3D ), ( const void* ) offsetof( Vertex3D, m_position ) );
	g_theRenderer->VennVertexAttribPointer( colorLocation,			4,	V_UNSIGNED_BYTE,	true,	sizeof( Vertex3D ), ( const void* ) offsetof( Vertex3D, m_color ) );
	g_theRenderer->VennVertexAttribPointer( texCoordLocation,		2,	V_FLOAT,			false,	sizeof( Vertex3D ), ( const void* ) offsetof( Vertex3D, m_textureCoords ) );
	g_theRenderer->VennVertexAttribPointer( normalCoordLocation,	3,	V_FLOAT,			false,	sizeof( Vertex3D ), ( const void* ) offsetof( Vertex3D, m_normal ) );
	g_theRenderer->VennVertexAttribPointer( tangentCoordLocation,	3,	V_FLOAT,			false,	sizeof( Vertex3D ), ( const void* ) offsetof( Vertex3D, m_tangent ) );
	g_theRenderer->VennVertexAttribPointer( bitangentCoordLocation,	3,	V_FLOAT,			false,	sizeof( Vertex3D ), ( const void* ) offsetof( Vertex3D, m_bitangent ) );

	g_theRenderer->VennDrawArrays( mode, 0, arraySize );

	g_theRenderer->VennDisableVertexAttribArray( positionLocation );
	g_theRenderer->VennDisableVertexAttribArray( colorLocation );
	g_theRenderer->VennDisableVertexAttribArray( texCoordLocation );
	g_theRenderer->VennDisableVertexAttribArray( normalCoordLocation );
	g_theRenderer->VennDisableVertexAttribArray( tangentCoordLocation );
	g_theRenderer->VennDisableVertexAttribArray( bitangentCoordLocation );

	g_theRenderer->VennBindBuffer( V_ARRAY_BUFFER, 0 );
}


//---------------------------------------------------------------------------
void RenderElements( const Vertex3D* vertices, const int* indices, const int indexCount, const VennEnum mode )
{
	g_theRenderer->VennEnableClientState( V_VERTEX_ARRAY );
	g_theRenderer->VennEnableClientState( V_COLOR_ARRAY );
	g_theRenderer->VennEnableClientState( V_TEXTURE_COORD_ARRAY );

	g_theRenderer->VennVertexPointer(	3,	V_FLOAT,			sizeof( Vertex3D ),		&vertices[0].m_position );
	g_theRenderer->VennColorPointer(	4,	V_UNSIGNED_BYTE,	sizeof( Vertex3D ),		&vertices[0].m_color );
	g_theRenderer->VennTexCoordPointer(	2,	V_FLOAT,			sizeof( Vertex3D ),		&vertices[0].m_textureCoords );

	g_theRenderer->VennDrawElements( mode, indexCount, V_UNSIGNED_INT, indices );

	g_theRenderer->VennDisableClientState( V_VERTEX_ARRAY );
	g_theRenderer->VennDisableClientState( V_COLOR_ARRAY );
	g_theRenderer->VennDisableClientState( V_TEXTURE_COORD_ARRAY );
}


//---------------------------------------------------------------------------
void RenderElementsWithVBOAndIBO( const unsigned int vboID, const unsigned int iboID, const unsigned int indexCount, const unsigned int shaderProgramID, const VennEnum mode )
{
	g_theRenderer->VennBindBuffer( V_ARRAY_BUFFER, vboID );

	//	glGetAttribLocation( shaderProgramID, "v_texCoords" );
	int uniformLocation = g_theRenderer->VennGetUniformLocation( shaderProgramID, "u_worldToClipSpaceMatrix" );
	g_theRenderer->VennUniformMatrix4fv( uniformLocation, 1, false, ( float* ) &g_theRenderer->GetCurrentMatrixData()[0] );
	//	m_multiTextureMaterial->RegisterDynamicUniformMatrix4( "u_worldToClipSpaceMatrix", g_theRenderer->GetCurrentMatrixData() );


	int positionLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_position" );
	int colorLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_color" );
	int texCoordLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_texCoords" );
	int normalCoordLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_normal" );
	int tangentCoordLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_tangent" );
	int bitangentCoordLocation = g_theRenderer->VennGetAttribLocation( shaderProgramID, "a_bitangent" );

	g_theRenderer->VennEnableVertexAttribArray( positionLocation );
	g_theRenderer->VennEnableVertexAttribArray( colorLocation );
	g_theRenderer->VennEnableVertexAttribArray( texCoordLocation );
	g_theRenderer->VennEnableVertexAttribArray( normalCoordLocation );
	g_theRenderer->VennEnableVertexAttribArray( tangentCoordLocation );
	g_theRenderer->VennEnableVertexAttribArray( bitangentCoordLocation );

	g_theRenderer->VennBindBuffer( V_ELEMENT_ARRAY_BUFFER, iboID );

	g_theRenderer->VennVertexAttribPointer( positionLocation,		3,	V_FLOAT,			false,	sizeof( Vertex3D ), ( const void* ) offsetof( Vertex3D, m_position ) );
	g_theRenderer->VennVertexAttribPointer( colorLocation,			4,	V_UNSIGNED_BYTE,	true,	sizeof( Vertex3D ), ( const void* ) offsetof( Vertex3D, m_color ) );
	g_theRenderer->VennVertexAttribPointer( texCoordLocation,		2,	V_FLOAT,			false,	sizeof( Vertex3D ), ( const void* ) offsetof( Vertex3D, m_textureCoords ) );
	g_theRenderer->VennVertexAttribPointer( normalCoordLocation,	3,	V_FLOAT,			false,	sizeof( Vertex3D ), ( const void* ) offsetof( Vertex3D, m_normal ) );
	g_theRenderer->VennVertexAttribPointer( tangentCoordLocation,	3,	V_FLOAT,			false,	sizeof( Vertex3D ), ( const void* ) offsetof( Vertex3D, m_tangent ) );
	g_theRenderer->VennVertexAttribPointer( bitangentCoordLocation,	3,	V_FLOAT,			false,	sizeof( Vertex3D ), ( const void* ) offsetof( Vertex3D, m_bitangent ) );

	//g_theRenderer->VennDrawArrays( mode, 0, arraySize );
	g_theRenderer->VennDrawElements( mode, indexCount, V_UNSIGNED_INT, ( void* ) 0 );

	g_theRenderer->VennDisableVertexAttribArray( positionLocation );
	g_theRenderer->VennDisableVertexAttribArray( colorLocation );
	g_theRenderer->VennDisableVertexAttribArray( texCoordLocation );
	g_theRenderer->VennDisableVertexAttribArray( normalCoordLocation );
	g_theRenderer->VennDisableVertexAttribArray( tangentCoordLocation );
	g_theRenderer->VennDisableVertexAttribArray( bitangentCoordLocation );

	g_theRenderer->VennBindBuffer( V_ARRAY_BUFFER, 0 );
	g_theRenderer->VennBindBuffer( V_ELEMENT_ARRAY_BUFFER, 0 );
}