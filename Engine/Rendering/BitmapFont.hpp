//---------------------------------------------------------------------------
// BitmapFont.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_BITMAPFONT_
#define _INCLUDED_BITMAPFONT_

#include <string>
#include <map>
#include <vector>
#include "Engine/Math/Vec2.hpp"

class Texture;
class ZipFile;
class TiXmlDocument;
struct GlyphMetaData;

/////////////////////////////////////////////////////////////////////////////
class BitmapFont
{
public:
	BitmapFont( const std::string& metaDataFilePath );
	BitmapFont( ZipFile* zipFile, const char* metaDataFilePath );
	~BitmapFont();

	static BitmapFont*				CreateAndGetFont( const std::string& metaDataFilePath );
	static BitmapFont*				CreateAndGetFont( ZipFile* zipFile, const char* metaDataFilePath );

public:
	std::vector< Texture* >			m_glyphSheets;
	std::map< int, GlyphMetaData >	m_glyphData;

private:
	BitmapFont();
	bool							LoadMetaDataWithTinyXML( const std::string& metaDataFilePath );
	bool							LoadMetaDataWithTinyXML( ZipFile* zipFile, const char* metaDataFilePath );
	void							ExtractMetaDataFromTinyXMLDocument( TiXmlDocument& metaDataDocument );
	bool							AddTexturesFromFolder( TiXmlDocument& metaDataDocument, const std::string& metaDataFilePath );
	bool							AddTexturesFromZip( TiXmlDocument& metaDataDocument, ZipFile* zipFile, const std::string& metaDataFilePath );

};


/////////////////////////////////////////////////////////////////////////////
struct GlyphMetaData
{
	Vec2f	m_minTexCoords;
	Vec2f	m_maxTexCoords;
	float	m_ttfA;
	float	m_ttfB;
	float	m_ttfC;
	char	m_glyphSheetIndex;
};

extern bool g_isQuitting;

#endif