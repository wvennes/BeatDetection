//---------------------------------------------------------------------------
// BitmapFont.cpp
//---------------------------------------------------------------------------

//#define	TIXML_USE_STL
//#pragma comment( lib, "./Code/TinyXML/tinyxml.lib" )
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Engine/External/TinyXML/tinyxml.h"
#include "Engine/Core/ZipFile.hpp"
#include "Engine/Rendering/Texture.hpp"
#include "Engine/Rendering/BitmapFont.hpp"

#define STATIC


//---------------------------------------------------------------------------
BitmapFont::BitmapFont()
{

}


//---------------------------------------------------------------------------
BitmapFont::BitmapFont( const std::string& metaDataFilePath )
{
	LoadMetaDataWithTinyXML( metaDataFilePath );
}


//---------------------------------------------------------------------------
BitmapFont::BitmapFont( ZipFile* zipFile, const char* metaDataFilePath )
{
	LoadMetaDataWithTinyXML( zipFile, metaDataFilePath );
}


//---------------------------------------------------------------------------
BitmapFont::~BitmapFont()
{

}


//---------------------------------------------------------------------------
STATIC BitmapFont* BitmapFont::CreateAndGetFont( const std::string& metaDataFilePath )
{
	BitmapFont* result = new BitmapFont();
	bool didLoad = result->LoadMetaDataWithTinyXML( metaDataFilePath );
	if ( !didLoad )
	{
		delete result;
		result = nullptr;
	}

	return result;
}


//---------------------------------------------------------------------------
STATIC BitmapFont* BitmapFont::CreateAndGetFont( ZipFile* zipFile, const char* metaDataFilePath )
{
	BitmapFont* result = new BitmapFont();
	bool didLoad = result->LoadMetaDataWithTinyXML( zipFile, metaDataFilePath );
	if ( !didLoad )
	{
		delete result;
		result = nullptr;
	}
	
	return result;
}


//---------------------------------------------------------------------------
bool BitmapFont::LoadMetaDataWithTinyXML( const std::string& metaDataFilePath )
{
	TiXmlDocument metaDataDocument;
	bool isMetadataLoaded = metaDataDocument.LoadFile( metaDataFilePath.c_str() );

	if ( !isMetadataLoaded )
	{
		MessageBoxA( nullptr, "TinyXML Error: Failed to load metadata.", "TinyXML Load Error", MB_OK );
		//g_isQuitting = true;
		return false;
	}

	if ( !AddTexturesFromFolder( metaDataDocument, metaDataFilePath ) ) return false;
	ExtractMetaDataFromTinyXMLDocument( metaDataDocument );
	return true;
}


//---------------------------------------------------------------------------
bool BitmapFont::LoadMetaDataWithTinyXML( ZipFile* zipFile, const char* metaDataFilePath )
{
	if ( !zipFile ) return false;
	size_t fileSize = 0;
	char* fileBuffer = zipFile->LoadFileToHeap( metaDataFilePath, fileSize );
	if ( !fileBuffer ) return false;
	std::string fileBufferAsString( fileBuffer, fileSize );
	TiXmlDocument metaDataDocument;
	metaDataDocument.Parse( fileBufferAsString.c_str(), 0, TIXML_ENCODING_UTF8 );

	if ( !AddTexturesFromZip( metaDataDocument, zipFile, metaDataFilePath ) ) return false;
	ExtractMetaDataFromTinyXMLDocument( metaDataDocument );

	delete fileBuffer;
	return true;
}


//---------------------------------------------------------------------------
void BitmapFont::ExtractMetaDataFromTinyXMLDocument( TiXmlDocument& metaDataDocument )
{
	Vec2f glyphPosition;
	Vec2f glyphDimensions;

	TiXmlElement* rootElement = metaDataDocument.RootElement();
	TiXmlElement* charElement = rootElement->FirstChildElement( "chars" );
	TiXmlElement* commonElement = rootElement->FirstChildElement( "common" );

	Vec2f glyphSheetDimensions( ( float ) atof( commonElement->Attribute( "scaleW" ) ), ( float ) atof( commonElement->Attribute( "scaleH" ) ) );
	Vec2f oneOverGlyphSheetDimensions( 1.f / glyphSheetDimensions.x, 1.f / glyphSheetDimensions.y );
	float oneOverGlyphCellHeight = 1.f / ( float ) atof( commonElement->Attribute( "lineHeight" ) );

	for ( TiXmlElement* currentElement = charElement->FirstChildElement( "char" ); currentElement != nullptr; currentElement = currentElement->NextSiblingElement( "char" ) )
	{
		GlyphMetaData& dataHolder = m_glyphData[ atoi( currentElement->Attribute( "id" ) ) ];
		// store page number
		dataHolder.m_glyphSheetIndex = ( char ) atoi( currentElement->Attribute( "page" ) );

		// find mins
		glyphPosition.x = ( float ) atof( currentElement->Attribute( "x" ) );
		glyphPosition.y = ( float ) atof( currentElement->Attribute( "y" ) );
		dataHolder.m_minTexCoords = Vec2f( glyphPosition.x * oneOverGlyphSheetDimensions.x, glyphPosition.y * oneOverGlyphSheetDimensions.y );

		// find maxes
		glyphDimensions.x = ( float ) atof( currentElement->Attribute( "width" ) ) * oneOverGlyphSheetDimensions.x;
		glyphDimensions.y = ( float ) atof( currentElement->Attribute( "height" ) ) * oneOverGlyphSheetDimensions.y;
		dataHolder.m_maxTexCoords = dataHolder.m_minTexCoords + glyphDimensions;

		// find a b c values
		dataHolder.m_ttfA = ( float ) atof( currentElement->Attribute( "xoffset" ) ) * oneOverGlyphCellHeight;
		dataHolder.m_ttfB = ( float ) atof( currentElement->Attribute( "width" ) ) * oneOverGlyphCellHeight; // width
		dataHolder.m_ttfC = ( float ) ( atoi( currentElement->Attribute( "xadvance" ) ) -
			( atoi( currentElement->Attribute( "width" ) ) + atoi( currentElement->Attribute( "xoffset" ) ) ) )
			* oneOverGlyphCellHeight; // xoffset - ( xadvance + width )
	}
}


//---------------------------------------------------------------------------
bool BitmapFont::AddTexturesFromFolder( TiXmlDocument& metaDataDocument, const std::string& metaDataFilePath )
{
	TiXmlElement* rootElement = metaDataDocument.RootElement();
	TiXmlElement* pageElement = rootElement->FirstChildElement( "pages" );
	const size_t lastSlashLocation = metaDataFilePath.rfind( '/' );
	for ( TiXmlElement* currentElement = pageElement->FirstChildElement( "page" ); currentElement != nullptr; currentElement = currentElement->NextSiblingElement( "page" ) )
	{
		Texture* texture = Texture::CreateOrGetTexture( metaDataFilePath.substr( 0, lastSlashLocation + 1 ) + std::string( currentElement->Attribute( "file" ) ) );
		if ( !texture ) return false;
		m_glyphSheets.push_back( texture );
	}
	return true;
}


//---------------------------------------------------------------------------
bool BitmapFont::AddTexturesFromZip( TiXmlDocument& metaDataDocument, ZipFile* zipFile, const std::string& metaDataFilePath )
{
	TiXmlElement* rootElement = metaDataDocument.RootElement();
	TiXmlElement* pageElement = rootElement->FirstChildElement( "pages" );
	const size_t lastSlashLocation = metaDataFilePath.rfind( '/' );
	for ( TiXmlElement* currentElement = pageElement->FirstChildElement( "page" ); currentElement != nullptr; currentElement = currentElement->NextSiblingElement( "page" ) )
	{
		Texture* texture = Texture::CreateOrGetTexture( zipFile, metaDataFilePath.substr( 0, lastSlashLocation + 1 ) + std::string( currentElement->Attribute( "file" ) ) );
		if ( !texture ) return false;
		m_glyphSheets.push_back( texture );
	}
	return true;
}