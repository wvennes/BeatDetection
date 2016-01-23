//---------------------------------------------------------------------------
// ProfileSection.cpp
//---------------------------------------------------------------------------

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Rendering/Material.hpp"
#include "Engine/Rendering/TextRenderer.hpp"
#include "Engine/Rendering/VennRenderer.hpp"

#include "Engine/Core/Profiler.hpp"

#define STATIC



//---------------------------------------------------------------------------
Profiler::Profiler()
	: m_overallSection( nullptr )
	, m_inputSection( nullptr )
	, m_updateSection( nullptr )
	, m_renderSection( nullptr )
{
	Initialize();
}


//---------------------------------------------------------------------------
Profiler::~Profiler()
{
	delete m_overallSection;
	delete m_inputSection;
	delete m_updateSection;
	delete m_renderSection;
}


//---------------------------------------------------------------------------
void Profiler::Initialize()
{
	m_overallSection = new ProfileSection( CATEGORY_OVERALL );
	m_inputSection = new ProfileSection( CATEGORY_INPUT );
	m_updateSection = new ProfileSection( CATEGORY_UPDATE );
	m_renderSection = new ProfileSection( CATEGORY_RENDER );
}


//---------------------------------------------------------------------------
void Profiler::StartProfileSection( ProfileCategory profileCategory )
{
	switch ( profileCategory )
	{
		case CATEGORY_OVERALL:
		{
			m_overallSection->StartProfiling();
			break;
		}
		case CATEGORY_INPUT:
		{
			m_inputSection->StartProfiling();
			break;
		}
		case CATEGORY_UPDATE:
		{
			m_updateSection->StartProfiling();
			break;
		}
		case CATEGORY_RENDER:
		{
			m_renderSection->StartProfiling();
			break;
		}
	}
}


//---------------------------------------------------------------------------
void Profiler::StopProfileSection( ProfileCategory profileCategory )
{
	switch ( profileCategory )
	{
		case CATEGORY_OVERALL:
		{
			m_overallSection->StopProfiling();
			break;
		}
		case CATEGORY_INPUT:
		{
			m_inputSection->StopProfiling();
			break;
		}
		case CATEGORY_UPDATE:
		{
			m_updateSection->StopProfiling();
			break;
		}
		case CATEGORY_RENDER:
		{
			m_renderSection->StopProfiling();
			break;
		}
	}
}


//---------------------------------------------------------------------------
Seconds Profiler::GetOverallTimeForSection( ProfileCategory profileCategory )
{
	switch ( profileCategory )
	{
		case CATEGORY_OVERALL:
		{
			return m_overallSection->GetTotalTimeForSection();
		}
		case CATEGORY_INPUT:
		{
			return m_inputSection->GetTotalTimeForSection();
		}
		case CATEGORY_UPDATE:
		{
			return m_updateSection->GetTotalTimeForSection();
		}
		case CATEGORY_RENDER:
		{
			return m_renderSection->GetTotalTimeForSection();
		}
	}

	return 0.0;
}


//---------------------------------------------------------------------------
Seconds Profiler::GetAverageTimeForSection( ProfileCategory profileCategory )
{
	switch ( profileCategory )
	{
		case CATEGORY_OVERALL:
		{
			return m_overallSection->GetAverageTimeForSection();
		}
		case CATEGORY_INPUT:
		{
			return m_inputSection->GetAverageTimeForSection();
		}
		case CATEGORY_UPDATE:
		{
			return m_updateSection->GetAverageTimeForSection();
		}
		case CATEGORY_RENDER:
		{
			return m_renderSection->GetAverageTimeForSection();
		}
	}

	return 0.0;
}


//---------------------------------------------------------------------------
double Profiler::GetTotalPercentageOfOverallTime( ProfileCategory profileCategory )
{
	double result = 0.0;
	double overallTotalTime = m_overallSection->GetTotalTimeForSection();
	double totalTimeToUse = 0.0;

	switch ( profileCategory )
	{
		case CATEGORY_OVERALL:
		{
			totalTimeToUse = m_overallSection->GetTotalTimeForSection();
			break;
		}
		case CATEGORY_INPUT:
		{
			totalTimeToUse = m_inputSection->GetTotalTimeForSection();
			break;
		}
		case CATEGORY_UPDATE:
		{
			totalTimeToUse = m_updateSection->GetTotalTimeForSection();
			break;
		}
		case CATEGORY_RENDER:
		{
			totalTimeToUse = m_renderSection->GetTotalTimeForSection();
			break;
		}
	}

	if ( overallTotalTime > 0.0 ) result = ( totalTimeToUse / overallTotalTime ) * 100.0;

	return result;
}


//---------------------------------------------------------------------------
double Profiler::GetAveragePercentageOfOverallTime( ProfileCategory profileCategory )
{
	double result = 0.0;
	double overallAverageTime = m_overallSection->GetAverageTimeForSection();
	double averageTimeToUse = 0.0;

	switch ( profileCategory )
	{
		case CATEGORY_OVERALL:
		{
			averageTimeToUse = m_overallSection->GetAverageTimeForSection();
			break;
		}
		case CATEGORY_INPUT:
		{
			averageTimeToUse = m_inputSection->GetAverageTimeForSection();
			break;
		}
		case CATEGORY_UPDATE:
		{
			averageTimeToUse = m_updateSection->GetAverageTimeForSection();
			break;
		}
		case CATEGORY_RENDER:
		{
			averageTimeToUse = m_renderSection->GetAverageTimeForSection();
			break;
		}
	}

	if ( overallAverageTime > 0.0 ) result = ( averageTimeToUse / overallAverageTime ) * 100.0;

	return result;
}


//---------------------------------------------------------------------------
void Profiler::Render( float cellHeight /* = 32.f */ )
{
	Material* mat = Material::GetDefaultUnlitMaterial();
	Vec2f screenPosition( 0.f, GAME_WINDOW_Y - cellHeight );

	g_theTextRenderer->TextRendererPrintf( cellHeight, screenPosition, Colors::WHITE, mat->GetShaderProgramID(),
		"-- PROFILING INFORMATION --" );

	screenPosition += Vec2f( cellHeight, -cellHeight );
	g_theTextRenderer->TextRendererPrintf( cellHeight, screenPosition, Colors::WHITE, mat->GetShaderProgramID(),
		"OVERALL TIME: %f, AVG TIME: %f", GetOverallTimeForSection( CATEGORY_OVERALL ), GetAverageTimeForSection( CATEGORY_OVERALL ) );

	// input section
	screenPosition += Vec2f( cellHeight, -cellHeight );
	g_theTextRenderer->TextRendererPrintf( cellHeight, screenPosition, Colors::WHITE, mat->GetShaderProgramID(),
		"-- INPUT --" );

	screenPosition += Vec2f( cellHeight, -cellHeight );
	RenderSectionInformation( screenPosition, cellHeight, CATEGORY_INPUT );
	screenPosition += Vec2f( -cellHeight, -cellHeight );

	// update section
	g_theTextRenderer->TextRendererPrintf( cellHeight, screenPosition, Colors::WHITE, mat->GetShaderProgramID(),
		"-- UPDATE --" );
	screenPosition += Vec2f( cellHeight, -cellHeight );
	RenderSectionInformation( screenPosition, cellHeight, CATEGORY_UPDATE );
	screenPosition += Vec2f( -cellHeight, -cellHeight );

	// render section
	g_theTextRenderer->TextRendererPrintf( cellHeight, screenPosition, Colors::WHITE, mat->GetShaderProgramID(),
		"-- RENDER --" );
	screenPosition += Vec2f( cellHeight, -cellHeight );
	RenderSectionInformation( screenPosition, cellHeight, CATEGORY_RENDER );
	screenPosition += Vec2f( -cellHeight, -cellHeight );
}


//---------------------------------------------------------------------------
void Profiler::RenderSectionInformation( Vec2f& screenPosition, float cellHeight, ProfileCategory profileSection )
{
	Material* mat = Material::GetDefaultUnlitMaterial();

	g_theTextRenderer->TextRendererPrintf( cellHeight, screenPosition, Colors::WHITE, mat->GetShaderProgramID(),
		"TOTAL TIME: %f, AVG TIME: %f", GetOverallTimeForSection( profileSection ), GetAverageTimeForSection( profileSection ) );

	screenPosition += Vec2f( 0.f, -cellHeight );

	g_theTextRenderer->TextRendererPrintf( cellHeight, screenPosition, Colors::WHITE, mat->GetShaderProgramID(),
		"PERCENT OF TOTAL: %i%%, AVG PERCENT: %i%%", ( int ) GetTotalPercentageOfOverallTime( profileSection ), ( int ) GetAveragePercentageOfOverallTime( profileSection ) );	
}