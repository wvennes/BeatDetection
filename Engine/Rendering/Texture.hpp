//-----------------------------------------------------------------------------------------------
// Texture.hpp
//-----------------------------------------------------------------------------------------------
#pragma once
#ifndef _INCLUDED_TEXTURE_
#define _INCLUDED_TEXTURE_

#include <map>
#include <string>
#include "Engine/Math/Vec2.hpp"

class VennRenderer;
class ZipFile;


/////////////////////////////////////////////////////////////////////////////
class Texture
{
public:
	static std::map< std::string, Texture* >	s_textureRegistry;
	unsigned int								m_textureID;
	Vec2i										m_size;

public:
	Texture( const std::string& stringFilePath );
	Texture( ZipFile* zipArchive, const char* filePath );
	Texture( unsigned char* imageData, Vec2i& size, int numComponents );
	~Texture();
	static Texture*								GetTextureByName( const std::string& imageFilePath );
	static Texture*								CreateOrGetTexture( const std::string& imageFilePath );
	static Texture*								CreateOrGetTexture( ZipFile* zipArchive, const std::string& imageFilePath );
	static void									GenerateDefaultTextures();
	static void									FreeTextureRegistryMemory();

private:
	Texture();
	void										GenerateOpenGLTexture( unsigned char* imageData, int numComponents, int numComponentsRequested );
	bool										LoadFileIfAble( const std::string& imageFilePath );
	bool										LoadFileIfAble( ZipFile* zipArchive, const char* imageFilePath );
	
	static void									GenerateDefaultDiffuseTexture();
	static void									GenerateDefaultNormalTexture();
	static void									GenerateDefaultSpecGlossEmissiveTexture();

};

extern VennRenderer* g_theRenderer;

#endif