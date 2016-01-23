//---------------------------------------------------------------------------
// TextRenderer.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_TEXTRENDERER_
#define _INCLUDED_TEXTRENDERER_

#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Rgba.hpp"

class ZipFile;
class BitmapFont;
class VennRenderer;

/////////////////////////////////////////////////////////////////////////////
class TextRenderer
{
public:
	BitmapFont*			m_bitmapFont;

public:
	TextRenderer( const char* fontMetaDataFilePath );
	TextRenderer( ZipFile* zipArchive, const char* fontMetaDataFilePath );
	~TextRenderer();

	static TextRenderer*	CreateAndGetTextRenderer( const char* fontMetaDataFilePath );
	static TextRenderer*	CreateAndGetTextRenderer( ZipFile* zipArchive, const char* fontMetaDataFilePath );

	void					RenderText2D( const char* text, const float cellHeight, const Vec2f& screenPosition, const Rgba& textColor, const unsigned int shaderProgramID );
	void					TextRendererPrintf( const float cellHeight, const Vec2f& screenPosition, const Rgba& color, const unsigned int shaderProgramID, const char* text, ... );
	void					RenderSingleCharInQuad( char glyph, const Vec2f& screenPosition, const Rgba& textColor, const unsigned int shaderProgramID );
	float					CalculateTextWidth( const char* text, const float cellHeight );

private:
	TextRenderer();
	void					TextRendererPrintf( const float cellHeight, const Vec2f& screenPosition, const Rgba& color, const unsigned int shaderProgramID, const char* text, char* args );

};

extern TextRenderer*		g_theTextRenderer;
extern VennRenderer*		g_theRenderer;

#endif