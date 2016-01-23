//---------------------------------------------------------------------------
// World.cpp
//---------------------------------------------------------------------------

#include "Engine/Math/GameMath.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/Rendering/VennRenderer.hpp"

#include "../BeatGame/GameCommon.hpp"
#include "../BeatGame/World.hpp"


//---------------------------------------------------------------------------
World::World( float innerRingRadius, float outerRingRadius )
	: m_ringResizeTimer( 0.f )
	, m_innerRingRadius( innerRingRadius )
	, m_outerRingRadius( outerRingRadius )
	, m_destinationRingRadius( innerRingRadius )
{
	Initialize();
}


//---------------------------------------------------------------------------
World::~World()
{

}


//---------------------------------------------------------------------------
void World::Initialize()
{
	unsigned int numDegrees = 360;
	m_innerRingCoords.reserve( numDegrees ); // 360 degrees
	m_outerRingCoords.reserve( numDegrees ); // 360 degrees

	for ( unsigned int index = 0; index < numDegrees; ++ index )
	{
		m_innerRingCoords.push_back( Vertex3D() );
		m_outerRingCoords.push_back( Vertex3D() );
	}

	UpdateRingCoords( m_innerRingCoords, m_innerRingRadius );
	UpdateRingCoords( m_outerRingCoords, m_outerRingRadius );
}


//---------------------------------------------------------------------------
bool World::IsRadiusAtMaximum()
{
	if ( m_destinationRingRadius >= m_outerRingRadius * 0.65f ) return true;

	return false;
}


//---------------------------------------------------------------------------
bool World::Update()
{
	static float destinationUpperBounds = m_outerRingRadius * 0.65f;
	static float destinationLowerBounds = 25.f;

	bool didUpdate = false;

	if ( m_destinationRingRadius > destinationUpperBounds )
	{
		m_destinationRingRadius = destinationUpperBounds;
	}
	else if ( m_destinationRingRadius < destinationLowerBounds )
	{
		m_destinationRingRadius = destinationLowerBounds;
	}


	if ( m_destinationRingRadius > m_innerRingRadius || m_destinationRingRadius < m_innerRingRadius )
	{
		float incrementsPerSecond = ( m_destinationRingRadius - m_innerRingRadius ) / RING_RESIZE_TIMER_SECONDS;
		m_innerRingRadius += incrementsPerSecond * DELTA_TIME;

		//if ( m_innerRingRadius > m_destinationRingRadius || m_innerRingRadius < m_destinationRingRadius ) m_innerRingRadius = m_destinationRingRadius;

		UpdateRingCoords( m_innerRingCoords, m_innerRingRadius );

		didUpdate = true;
	}

	return didUpdate;
}


//---------------------------------------------------------------------------
void World::UpdateRingCoords( std::vector< Vertex3D >& ringCoords, float radius )
{
	Degrees currentDegrees = 0.f;
	for ( unsigned int index = 0; index < ( unsigned int ) ringCoords.size(); ++ index )
	{
		Vec3f& currentCoords = ringCoords[ index ].m_position;
		currentDegrees = ( float ) index;
		currentCoords = Vec3f( CosDegrees( currentDegrees ), SinDegrees( currentDegrees ), 0.f ) * radius;
	}
}


//---------------------------------------------------------------------------
void World::Render( Material* mat )
{
	//Material* mat = Material::GetDefaultUnlitMaterial(); // get the default for now... this will change.
	RenderVertexArrayWithShader( m_innerRingCoords.data(), m_innerRingCoords.size(), mat->GetShaderProgramID(), V_LINE_LOOP );
	RenderVertexArrayWithShader( m_outerRingCoords.data(), m_outerRingCoords.size(), mat->GetShaderProgramID(), V_LINE_LOOP );
}