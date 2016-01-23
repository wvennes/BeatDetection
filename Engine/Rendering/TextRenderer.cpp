//---------------------------------------------------------------------------
// TextRenderer.cpp
//---------------------------------------------------------------------------

#include "Engine/Rendering/Texture.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ZipFile.hpp"
#include "Engine/Rendering/RenderFunctions.hpp"
#include "Engine/Rendering/Vertex3D.hpp"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/BitmapFont.hpp"
#include "Engine/Rendering/TextRenderer.hpp"

#define STATIC


//---------------------------------------------------------------------------
TextRenderer::TextRenderer()
	: m_bitmapFont( nullptr )
{

}


//---------------------------------------------------------------------------
TextRenderer::TextRenderer( const char* fontMetaDataFilePath )
	: m_bitmapFont( nullptr )
{
	m_bitmapFont = new BitmapFont( fontMetaDataFilePath );
}


//---------------------------------------------------------------------------
TextRenderer::TextRenderer( ZipFile* zipArchive, const char* fontMetaDataFilePath )
	: m_bitmapFont( nullptr )
{
	m_bitmapFont = new BitmapFont( zipArchive, fontMetaDataFilePath );
}


//---------------------------------------------------------------------------
TextRenderer::~TextRenderer()
{
	if ( m_bitmapFont ) delete m_bitmapFont;
}


//---------------------------------------------------------------------------
STATIC TextRenderer* TextRenderer::CreateAndGetTextRenderer( const char* fontMetaDataFilePath )
{
	TextRenderer* result = nullptr;

	result = new TextRenderer();
	result->m_bitmapFont = BitmapFont::CreateAndGetFont( fontMetaDataFilePath );
	if ( !result->m_bitmapFont )
	{
		delete result;
		result = nullptr;
	}

	return result;
}


//---------------------------------------------------------------------------
STATIC TextRenderer* TextRenderer::CreateAndGetTextRenderer( ZipFile* zipArchive, const char* fontMetaDataFilePath )
{
	TextRenderer* result = nullptr;

	result = new TextRenderer();
	result->m_bitmapFont = BitmapFont::CreateAndGetFont( zipArchive, fontMetaDataFilePath );
	if ( !result->m_bitmapFont )
	{
		delete result;
		result = nullptr;
	}

	return result;
}


//---------------------------------------------------------------------------
void TextRenderer::RenderText2D( const char* text, const float cellHeight, const Vec2f& screenPosition, const Rgba& textColor, const unsigned int shaderProgramID )
{
	Vec2f glyphMins = screenPosition;
	Vec2f glyphMaxes( screenPosition.x, screenPosition.y + cellHeight );
	static Vertex3D vertices[ 4 ];
	//vertices.reserve( 4 );
	std::vector< Texture* >& glyphSheets = m_bitmapFont->m_glyphSheets;
	std::map< int, GlyphMetaData >& glyphMetaData = m_bitmapFont->m_glyphData;
	//std::string textString( text );

// 	g_theRenderer->VennLoadIdentity();
// 	g_theRenderer->VennOrtho( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
//	g_theRenderer->ApplyOrthoProjection( 0.f, GAME_WINDOW_X, 0.f, GAME_WINDOW_Y, 0.f, 1.f );
	g_theRenderer->VennEnable( V_TEXTURE_2D );
	g_theRenderer->VennBlendFunc( V_SRC_ALPHA, V_ONE_MINUS_SRC_ALPHA );

	for( unsigned int index = 0; index < strlen( text ); ++index )
	{
		GlyphMetaData& currentGlyphData = glyphMetaData[ text[ index ] ];
		Vec2f& minTexCoords = currentGlyphData.m_minTexCoords;
		Vec2f& maxTexCoords = currentGlyphData.m_maxTexCoords;

		g_theRenderer->VennBindTexture( V_TEXTURE_2D, glyphSheets[ currentGlyphData.m_glyphSheetIndex ]->m_textureID );

		glyphMins.x += ( cellHeight * currentGlyphData.m_ttfA );
		glyphMaxes.x = glyphMins.x + ( cellHeight * currentGlyphData.m_ttfB );

		vertices[ 0 ] = Vertex3D(
			Vec3f( glyphMins.x, glyphMins.y, 0.f ),
			textColor,
			Vec2f( minTexCoords.x, maxTexCoords.y ) );

		vertices[ 1 ] = Vertex3D(
			Vec3f( glyphMaxes.x, glyphMins.y, 0.f ),
			textColor,
			Vec2f( maxTexCoords.x, maxTexCoords.y ) );

		vertices[ 2 ] =  Vertex3D(
			Vec3f( glyphMaxes.x, glyphMaxes.y, 0.f ),
			textColor,
			Vec2f( maxTexCoords.x, minTexCoords.y ) );

		vertices[ 3 ] =  Vertex3D(
			Vec3f( glyphMins.x, glyphMaxes.y, 0.f ),
			textColor,
			Vec2f( minTexCoords.x, minTexCoords.y ) );

		glyphMins.x = glyphMaxes.x;
		glyphMins.x += ( cellHeight * currentGlyphData.m_ttfC );

		// render glyph
		//RenderVertexArray( vertices.data(), vertices.size(), V_QUADS );
		RenderVertexArrayWithShader( vertices, 4, shaderProgramID, V_QUADS );
	}
// 	std::vector< Vertex3D >().swap( vertices );
// 	textString.clear();
	g_theRenderer->VennBlendFunc( V_ONE, V_ONE_MINUS_SRC_ALPHA );
	g_theRenderer->VennDisable( V_TEXTURE_2D );
}


//---------------------------------------------------------------------------
void TextRenderer::TextRendererPrintf( const float cellHeight, const Vec2f& screenPosition, const Rgba& color, const unsigned int shaderProgramID, const char* text, ... )
{
	va_list args;
	va_start( args, text );
	{
		TextRendererPrintf( cellHeight, screenPosition, color, shaderProgramID, text, args );
	}
	va_end( args );
}


//---------------------------------------------------------------------------
void TextRenderer::TextRendererPrintf( const float cellHeight, const Vec2f& screenPosition, const Rgba& color, const unsigned int shaderProgramID, const char* text, va_list args )
{
	static const unsigned int BUFFER_SIZE = 2048;
	static char buffer[ BUFFER_SIZE ];
	vsnprintf_s( buffer, BUFFER_SIZE, BUFFER_SIZE - 1, text, args );
	RenderText2D( buffer, cellHeight, screenPosition, color, shaderProgramID );
}


//---------------------------------------------------------------------------
void TextRenderer::RenderSingleCharInQuad( char glyph, const Vec2f& screenPosition, const Rgba& textColor, const unsigned int shaderProgramID )
{
	float cellHeight = 1.f;
	Vec2f glyphMins = screenPosition;
	Vec2f glyphMaxes( screenPosition.x, screenPosition.y + cellHeight );
	std::vector< Vertex3D > vertices;
	std::vector< Texture* >& glyphSheets = m_bitmapFont->m_glyphSheets;
	std::map< int, GlyphMetaData >& glyphMetaData = m_bitmapFont->m_glyphData;

	vertices.reserve( 4 );

	GlyphMetaData& currentGlyphData = glyphMetaData[ glyph ];
	Vec2f& minTexCoords = currentGlyphData.m_minTexCoords;
	Vec2f& maxTexCoords = currentGlyphData.m_maxTexCoords;

	g_theRenderer->VennBindTexture( V_TEXTURE_2D, glyphSheets[ currentGlyphData.m_glyphSheetIndex ]->m_textureID );

	glyphMins.x += ( cellHeight * currentGlyphData.m_ttfA );
	glyphMaxes.x = glyphMins.x + ( cellHeight * currentGlyphData.m_ttfB );

	vertices.push_back( Vertex3D(
		Vec3f( glyphMins.x, glyphMins.y, 0.f ),
		textColor,
		Vec2f( minTexCoords.x, maxTexCoords.y ) ) );

	vertices.push_back( Vertex3D(
		Vec3f( glyphMaxes.x, glyphMins.y, 0.f ),
		textColor,
		Vec2f( maxTexCoords.x, maxTexCoords.y ) ) );

	vertices.push_back( Vertex3D(
		Vec3f( glyphMaxes.x, glyphMaxes.y, 0.f ),
		textColor,
		Vec2f( maxTexCoords.x, minTexCoords.y ) ) );

	vertices.push_back( Vertex3D(
		Vec3f( glyphMins.x, glyphMaxes.y, 0.f ),
		textColor,
		Vec2f( minTexCoords.x, minTexCoords.y ) ) );

	RenderVertexArrayWithShader( vertices.data(), vertices.size(), shaderProgramID, V_QUADS );

	vertices.clear();
	std::vector< Vertex3D >().swap( vertices );
	g_theRenderer->VennBlendFunc( V_ONE, V_ONE_MINUS_SRC_ALPHA );
	g_theRenderer->VennDisable( V_TEXTURE_2D );
}


//---------------------------------------------------------------------------
float TextRenderer::CalculateTextWidth( const char* text, const float cellHeight )
{
	std::string textString( text );
	std::map< int, GlyphMetaData >& glyphMetaData = m_bitmapFont->m_glyphData;
	float result = 0.f;

	for ( unsigned int index = 0; index < textString.length(); ++index )
	{
		GlyphMetaData& currentGlyphData = glyphMetaData[ textString[ index ] ];

		result += cellHeight * ( currentGlyphData.m_ttfA + currentGlyphData.m_ttfB + currentGlyphData.m_ttfC );
	}

	return result;
}