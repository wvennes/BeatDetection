//---------------------------------------------------------------------------
// DebugDraw.cpp
//---------------------------------------------------------------------------

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include "Vertex3D.hpp"
#include "VennRenderer.hpp"
#include "DebugDraw.hpp"

std::vector< DebugDrawObject* >		g_drawObjects;


//---------------------------------------------------------------------------
void DrawDebugVertexArray( const Vertex3D* vertices, const int arraySize, const VennEnum mode )
{
	g_theRenderer->VennEnableClientState( V_VERTEX_ARRAY );
	g_theRenderer->VennEnableClientState( V_COLOR_ARRAY );

	g_theRenderer->VennVertexPointer( 3, V_FLOAT, sizeof( Vertex3D ), &vertices[0].m_position );
	g_theRenderer->VennColorPointer( 4, V_FLOAT, sizeof( Vertex3D ), &vertices[0].m_color );

	g_theRenderer->VennDrawArrays( mode, 0, arraySize );

	g_theRenderer->VennDisableClientState( V_VERTEX_ARRAY );
	g_theRenderer->VennDisableClientState( V_COLOR_ARRAY );
}


//---------------------------------------------------------------------------
void DrawDebugPoint( const Vec3f& position, const Rgba& color, const float radius, const DrawMode drawMode, float durationSeconds )
{
	if ( durationSeconds > 0.f )
	{
		g_drawObjects.push_back( new DebugPoint( position, color, radius, drawMode, durationSeconds ) );
		return;
	}

	Vertex3D vertices[ 8 ];

	// add vertices for an asterisk ( * ) styled point in 3d space
	// line 1
	vertices[0].m_position = Vec3f( position.x - radius, position.y, position.z - radius );
	vertices[0].m_color = color;

	vertices[1].m_position = Vec3f( position.x + radius, position.y, position.z + radius );
	vertices[1].m_color = color;

	// line 2
	vertices[2].m_position = Vec3f( position.x - radius, position.y, position.z + radius );
	vertices[2].m_color = color;

	vertices[3].m_position = Vec3f( position.x + radius, position.y, position.z - radius );
	vertices[3].m_color = color;

	// line 3
	vertices[4].m_position = Vec3f( position.x, position.y - radius, position.z - radius );
	vertices[4].m_color = color;

	vertices[5].m_position = Vec3f( position.x, position.y + radius, position.z + radius );
	vertices[5].m_color = color;

	// line 4
	vertices[6].m_position = Vec3f( position.x, position.y - radius, position.z + radius );
	vertices[6].m_color = color;

	vertices[7].m_position = Vec3f( position.x, position.y + radius, position.z - radius );
	vertices[7].m_color = color;

	if ( drawMode == DEPTH_TEST_OFF || drawMode == DUAL_MODE )
	{
		g_theRenderer->VennDisable( V_DEPTH_TEST );
	}

	DrawDebugVertexArray( vertices, 8, V_LINES );

	if ( drawMode == DEPTH_TEST_OFF || drawMode == DUAL_MODE )
	{
		g_theRenderer->VennEnable( V_DEPTH_TEST );
	}

	if ( drawMode == DUAL_MODE )
	{
		g_theRenderer->VennLineWidth( 5.f );
		DrawDebugVertexArray( vertices, 8, V_LINES );
		g_theRenderer->VennLineWidth( 1.f );
	}

	glEnable( GL_POINTS );
	glVertex3f( position.x, position.y, position.z );
	glDisable( GL_POINTS );
}


//---------------------------------------------------------------------------
void DrawDebugLine( const Vec3f& startPosition, const Vec3f& endPosition, const Rgba& startColor, const Rgba& endColor, const DrawMode drawMode, float durationSeconds )
{
	if ( durationSeconds > 0.f )
	{
		g_drawObjects.push_back( new DebugLine( startPosition, endPosition, startColor, endColor, drawMode, durationSeconds ) );
		return;
	}

	Vertex3D vertices[2];

	vertices[0].m_position = startPosition;
	vertices[0].m_color = startColor;

	vertices[1].m_position = endPosition;
	vertices[1].m_color = endColor;

	if ( drawMode == DEPTH_TEST_OFF || drawMode == DUAL_MODE )
	{
		g_theRenderer->VennDisable( V_DEPTH_TEST );
	}

	DrawDebugVertexArray( vertices, 2, V_LINES );

	if ( drawMode == DEPTH_TEST_OFF || drawMode == DUAL_MODE )
	{
		g_theRenderer->VennEnable( V_DEPTH_TEST );
	}

	if ( drawMode == DUAL_MODE )
	{
		g_theRenderer->VennLineWidth( 5.f );
		DrawDebugVertexArray( vertices, 2, V_LINES );
		g_theRenderer->VennLineWidth( 1.f );
	}
}


//---------------------------------------------------------------------------
void DrawDebugArrow( const Vec3f& startPosition, const Vec3f& endPosition, const Rgba& startColor, const Rgba& endColor, const DrawMode drawMode, float durationSeconds )
{
	if ( durationSeconds > 0.f )
	{
		g_drawObjects.push_back( new DebugArrow( startPosition, endPosition, startColor, endColor, drawMode, durationSeconds ) );
		return;
	}

	Vertex3D vertices[ 2 ];
	float arrowScale = 0.05f;
// 	Vec3f direction = endPosition - startPosition;
// 	direction.Normalize();
// 	Vec3f worldUp = Vec3f( 0.f, 0.f, 1.f );
// 
// 	Vec3f perpendicular = direction.CrossProduct( worldUp );

	vertices[0].m_position = startPosition;
	vertices[0].m_color = startColor;

	vertices[1].m_position = endPosition;
	vertices[1].m_color = endColor;
	// 
	// 	vertices[2].m_position = endPosition;
	// 	vertices[2].m_color = endColor;
	// 
	// 	vertices[3].m_position = direction + ( perpendicular * arrowScale );
	// 	vertices[3].m_color = endColor;
	// 
	// 	vertices[4].m_position = endPosition;
	// 	vertices[4].m_color = endColor;
	// 
	// 	vertices[5].m_position = direction - ( perpendicular * arrowScale );
	// 	vertices[5].m_color = endColor;

	// 	if ( drawMode == DEPTH_TEST_ON || drawMode == DUAL_MODE )
	// 	{
	// 		g_theRenderer->VennEnable( V_DEPTH_TEST );
	// 	}
	if ( drawMode == DEPTH_TEST_OFF || drawMode == DUAL_MODE )
	{
		g_theRenderer->VennDisable( V_DEPTH_TEST );
	}

	DrawDebugVertexArray( vertices, 2, V_LINES );

	if ( drawMode == DEPTH_TEST_OFF || drawMode == DUAL_MODE )
	{
		g_theRenderer->VennEnable( V_DEPTH_TEST );
	}

	if ( drawMode == DUAL_MODE )
	{
		g_theRenderer->VennLineWidth( 5.f );
		DrawDebugVertexArray( vertices, 2, V_LINES );
		g_theRenderer->VennLineWidth( 1.f );
	}
	DrawDebugPoint( endPosition, endColor, arrowScale, drawMode );
}


//---------------------------------------------------------------------------
void DrawDebugAABB( const Vec3f& mins, const Vec3f& maxes, const Rgba& edgeColor, const Rgba& faceColor, const DrawMode drawMode, float durationSeconds )
{
	if ( durationSeconds > 0.f )
	{
		g_drawObjects.push_back( new DebugAABB( mins, maxes, edgeColor, faceColor, drawMode, durationSeconds ) );
		return;
	}

	Vertex3D cubeVertices[24];
	Vertex3D quadVertices[4];

	// bottom face
	cubeVertices[0].m_position = Vec3f( mins.x, mins.y, mins.z );
	cubeVertices[1].m_position = Vec3f( mins.x, maxes.y, mins.z );
	cubeVertices[2].m_position = Vec3f( maxes.x, maxes.y, mins.z );
	cubeVertices[3].m_position = Vec3f( maxes.x, mins.y, mins.z );

	// front face
	cubeVertices[4].m_position = Vec3f( mins.x, mins.y, mins.z );
	cubeVertices[5].m_position = Vec3f( mins.x, mins.y, maxes.z );
	cubeVertices[6].m_position = Vec3f( maxes.x, mins.y, maxes.z );
	cubeVertices[7].m_position = Vec3f( maxes.x, mins.y, mins.z );

	// left face
	cubeVertices[8].m_position = Vec3f( mins.x, mins.y, mins.z );
	cubeVertices[9].m_position = Vec3f( mins.x, maxes.y, mins.z );
	cubeVertices[10].m_position = Vec3f( mins.x, maxes.y, maxes.z );
	cubeVertices[11].m_position = Vec3f( mins.x, mins.y, maxes.z );

	// right face
	cubeVertices[12].m_position = Vec3f( maxes.x, mins.y, mins.z );
	cubeVertices[13].m_position = Vec3f( maxes.x, maxes.y, mins.z );
	cubeVertices[14].m_position = Vec3f( maxes.x, maxes.y, maxes.z );
	cubeVertices[15].m_position = Vec3f( maxes.x, mins.y, maxes.z );

	// back face
	cubeVertices[16].m_position = Vec3f( mins.x, maxes.y, mins.z );
	cubeVertices[17].m_position = Vec3f( maxes.x, maxes.y, mins.z );
	cubeVertices[18].m_position = Vec3f( maxes.x, maxes.y, maxes.z );
	cubeVertices[19].m_position = Vec3f( mins.x, maxes.y, maxes.z );

	// top face
	cubeVertices[20].m_position = Vec3f( mins.x, mins.y, maxes.z );
	cubeVertices[21].m_position = Vec3f( mins.x, maxes.y, maxes.z );
	cubeVertices[22].m_position = Vec3f( maxes.x, maxes.y, maxes.z );
	cubeVertices[23].m_position = Vec3f( maxes.x, mins.y, maxes.z );

	//g_theRenderer->VennDisable( V_CULL_FACE );

	for ( int index = 0; index < 24; ++index )
	{
		cubeVertices[index].m_color = faceColor;
	}

// 	if ( drawMode == DEPTH_TEST_OFF || drawMode == DUAL_MODE )
// 	{
// 		g_theRenderer->VennDisable( V_DEPTH_TEST );
// 	}
	DrawDebugVertexArray( cubeVertices, 24, V_QUADS );
// 	if ( drawMode == DEPTH_TEST_OFF || drawMode == DUAL_MODE )
// 	{
// 		g_theRenderer->VennEnable( V_DEPTH_TEST );
// 	}

	for ( int index = 0; index < 24; ++index )
	{
		cubeVertices[index].m_color = edgeColor;
	}

	// outlines
	int currentIndex = 0;
	for ( int count = 0; count < 6; ++count )
	{
		quadVertices[0].m_position = cubeVertices[currentIndex].m_position;
		quadVertices[0].m_color = cubeVertices[currentIndex].m_color;
		++currentIndex;

		quadVertices[1].m_position = cubeVertices[currentIndex].m_position;
		quadVertices[1].m_color = cubeVertices[currentIndex].m_color;
		++currentIndex;

		quadVertices[2].m_position = cubeVertices[currentIndex].m_position;
		quadVertices[2].m_color = cubeVertices[currentIndex].m_color;
		++currentIndex;

		quadVertices[3].m_position = cubeVertices[currentIndex].m_position;
		quadVertices[3].m_color = cubeVertices[currentIndex].m_color;
		++currentIndex;

		if ( drawMode == DEPTH_TEST_OFF || drawMode == DUAL_MODE )
		{
			g_theRenderer->VennDisable( V_DEPTH_TEST );
		}

		DrawDebugVertexArray( quadVertices, 4, V_LINE_LOOP );

		if ( drawMode == DEPTH_TEST_OFF || drawMode == DUAL_MODE )
		{
			g_theRenderer->VennEnable( V_DEPTH_TEST );
		}

		if ( drawMode == DUAL_MODE )
		{
			g_theRenderer->VennLineWidth( 5.f );
			DrawDebugVertexArray( quadVertices, 4, V_LINE_LOOP );
			g_theRenderer->VennLineWidth( 1.f );
		}
	}
}


//---------------------------------------------------------------------------
void DrawDebugSphere( const Vec3f& center, const Rgba& color, const float radius, const DrawMode drawMode, float durationSeconds )
{
	if ( durationSeconds > 0.f )
	{
		g_drawObjects.push_back( new DebugSphere( center, color, radius, drawMode, durationSeconds ) );
		return;
	}

	std::vector< Vertex3D > vertices;
	Vec3f currentPosition;

	float divisions = 35.f;

	float changeInTheta = ( float ) ( M_PI / divisions );
	float changeInPhi = ( float ) (	( 2 * M_PI ) / divisions );

	for ( float angleTheta = 0.f; angleTheta < ( float ) M_PI; angleTheta += changeInTheta )
	{
		for ( float anglePhi = 0.f; anglePhi < ( float ) ( 2 * M_PI ); anglePhi += changeInPhi )
		{
			currentPosition.x = radius * cos( angleTheta ) * sin( anglePhi ) + center.x;
			currentPosition.y = radius * sin( angleTheta ) * sin( anglePhi ) + center.y;
			currentPosition.z = radius * cos( anglePhi ) + center.z;

			vertices.push_back( Vertex3D( 
				currentPosition,
				color,
				Vec2f( 0.f, 0.f ) ) );

// 			currentPosition.x = radius * cos( angleTheta + changeInTheta ) * sin( anglePhi ) + center.x;
// 			currentPosition.y = radius * sin( angleTheta + changeInTheta ) * sin( anglePhi ) + center.y;
// 			currentPosition.z = radius * cos( anglePhi ) + center.z;
// 
// 			vertices.push_back( Vertex3Df( 
// 				currentPosition,
// 				color,
// 				Vec2f( 0.f, 0.f ) ) );
		}
	}

	if ( drawMode == DEPTH_TEST_OFF || drawMode == DUAL_MODE )
	{
		g_theRenderer->VennDisable( V_DEPTH_TEST );
	}

	DrawDebugVertexArray( vertices.data(), vertices.size(), V_POINTS );

	if ( drawMode == DEPTH_TEST_OFF || drawMode == DUAL_MODE )
	{
		g_theRenderer->VennEnable( V_DEPTH_TEST );
	}

	if ( drawMode == DUAL_MODE )
	{
		g_theRenderer->VennPointSize( 5.f );
		DrawDebugVertexArray( vertices.data(), vertices.size(), V_POINTS );
		g_theRenderer->VennPointSize( 1.f );
	}
}