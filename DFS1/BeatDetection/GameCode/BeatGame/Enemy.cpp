//---------------------------------------------------------------------------
// Enemy.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/GameMath.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/Rendering/VennRenderer.hpp"

#include "BeatGame/Enemy.hpp"

#define STATIC

STATIC float		Enemy::s_enemySpeedUnitsPerSecond = 25.f;
STATIC const float	Enemy::s_enemySizeRadius = 5.f;


//---------------------------------------------------------------------------
Enemy::Enemy( float innerRadius, float outerRadius, float directionDegrees )
	: Actor()
	, m_innerRingRadius( innerRadius )
	, m_outerRingRadius( outerRadius )
	, m_directionDegrees( directionDegrees )
{
	m_speedUnitsPerSecond = s_enemySpeedUnitsPerSecond;
	m_hitboxRadius = s_enemySizeRadius;

	m_vertexes.reserve( 4 );
	for ( unsigned int count = 0; count < 4; ++ count )
	{
		m_vertexes.push_back( Vertex3D() );
	}
	m_vertexes[ 0 ].m_position = Vec3f( -1.f, -1.f, 0.f );
	m_vertexes[ 1 ].m_position = Vec3f( 1.f, -1.f, 0.f );
	m_vertexes[ 2 ].m_position = Vec3f( 1.f, 1.f, 0.f );
	m_vertexes[ 3 ].m_position = Vec3f( -1.f, 1.f, 0.f );

	m_direction = Vec2f( CosDegrees( m_directionDegrees ), SinDegrees( m_directionDegrees ) );
	m_position = m_direction * m_outerRingRadius;
}


//---------------------------------------------------------------------------
Enemy::~Enemy()
{

}


//---------------------------------------------------------------------------
STATIC void Enemy::IncrementEnemySpeed( float speedUnitsPerSecond )
{
	s_enemySpeedUnitsPerSecond += speedUnitsPerSecond;
}


//---------------------------------------------------------------------------
bool Enemy::Update()
{
	m_position -= m_direction * m_speedUnitsPerSecond * DELTA_TIME;
	return m_isDead;
}


//---------------------------------------------------------------------------
void Enemy::Render( Material* mat )
{
	g_theRenderer->PushMatrix();
	{
		g_theRenderer->ApplyTranslation( Vec3f( m_position.x, m_position.y, 0.f ) );
		g_theRenderer->ApplyRotation( m_directionDegrees, Vec3f( 0.f, 0.f, 1.f ) );
		g_theRenderer->ApplyScaling( m_hitboxRadius );

		RenderVertexArrayWithShader( m_vertexes.data(), m_vertexes.size(), mat->GetShaderProgramID(), V_QUADS );
	}
	g_theRenderer->PopMatrix();
}