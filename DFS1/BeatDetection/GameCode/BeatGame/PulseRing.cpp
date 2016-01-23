//---------------------------------------------------------------------------
// PulseRing.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/GameMath.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "BeatGame/PulseRing.hpp"


//---------------------------------------------------------------------------
PulseRing::PulseRing( const Vec2f& position, float radius, float radiusIncrementPerSecond, float lifetimeSeconds, const Rgba& color )
	: Explosion( position, radius, radiusIncrementPerSecond, lifetimeSeconds, color )
{
	Vertex3D vertex;
	m_vertexes.reserve( 360 );
	for ( unsigned int count = 0; count < 360; ++ count )
	{
		vertex.m_position = Vec3f( CosDegrees( ( float ) count ), SinDegrees( ( float ) count ), 0.f );
		m_vertexes.push_back( vertex );
	}

	float dimness = 0.25f;

	m_color.r = ( unsigned char ) ( ( float ) m_color.r * dimness );
	m_color.g = ( unsigned char ) ( ( float ) m_color.g * dimness );
	m_color.b = ( unsigned char ) ( ( float ) m_color.b * dimness );
	m_color.a = ( unsigned char ) ( ( float ) m_color.a * dimness );
}


//---------------------------------------------------------------------------
PulseRing::~PulseRing()
{

}


//---------------------------------------------------------------------------
bool PulseRing::Update()
{
	Rgba color = m_color;
	float percentOfLifetime = 1.f - ( m_currentSeconds / m_lifetimeSeconds );

	color.r = ( unsigned char ) ( ( float ) m_color.r * percentOfLifetime );
	color.g = ( unsigned char ) ( ( float ) m_color.g * percentOfLifetime );
	color.b = ( unsigned char ) ( ( float ) m_color.b * percentOfLifetime );
	color.a = ( unsigned char ) ( ( float ) m_color.a * percentOfLifetime );

	// update colors
	for ( unsigned int index = 0; index < ( unsigned int ) m_vertexes.size(); ++ index )
	{
		m_vertexes[ index ].m_color = color;
	}

	m_currentRadius += m_radiusIncrementsPerSecond * DELTA_TIME;

	return Explosion::Update();
}


//---------------------------------------------------------------------------
void PulseRing::Render( Material* mat )
{
	g_theRenderer->PushMatrix();
	{
		g_theRenderer->ApplyScaling( m_currentRadius );
		RenderVertexArrayWithShader( m_vertexes.data(), m_vertexes.size(), mat->GetShaderProgramID(), V_LINE_LOOP );
	}
	g_theRenderer->PopMatrix();
}