//---------------------------------------------------------------------------
// Texture.cpp - Sample template code by Squirrel Eiserloh
//---------------------------------------------------------------------------
#define STBI_HEADER_FILE_ONLY
#include "Engine/Core/ZipFile.hpp"
#include "Engine/Rendering/stb_image.c"
#include "Engine/Rendering/VennRenderer.hpp"
#include "Engine/Rendering/Texture.hpp"
#define UNUSED( x ) ( void )( x )

#define STATIC // Do-nothing indicator that method/member is static in class definition


STATIC std::map< std::string, Texture* >	Texture::s_textureRegistry;


//---------------------------------------------------------------------------
Texture::Texture()
	: m_textureID( 0 )
	, m_size( 0, 0 )
{

}


//---------------------------------------------------------------------------
Texture::Texture( const std::string& imageFilePath )
	: m_textureID( 0 )
	, m_size( 0, 0 )
{
	int numComponents = 0; // Filled in for us to indicate how many color/alpha components the image had (e.g. 3=RGB, 4=RGBA)
	int numComponentsRequested = 0; // don't care; we support 3 (RGB) or 4 (RGBA)
	unsigned char* imageData = stbi_load( imageFilePath.c_str(), &m_size.x, &m_size.y, &numComponents, numComponentsRequested );

	// Enable texturing
	//glEnable( V_TEXTURE_2D );
	GenerateOpenGLTexture( imageData, numComponents, numComponentsRequested );

	stbi_image_free( imageData );
}


//---------------------------------------------------------------------------
Texture::Texture( ZipFile* zipArchive, const char* filePath )
	: m_textureID( 0 )
	, m_size( 0, 0 )
{
	int numComponents = 0;
	int numComponentsRequested = 0;
	size_t fileBufferLength = 0;
	unsigned char* fileBuffer = reinterpret_cast< unsigned char* > ( zipArchive->LoadFileToHeap( filePath, fileBufferLength ) );
	unsigned char* imageData = stbi_load_from_memory( fileBuffer, ( int ) fileBufferLength, &m_size.x, &m_size.y, &numComponents, numComponentsRequested );
	
	GenerateOpenGLTexture( imageData, numComponents, numComponentsRequested );

	stbi_image_free( imageData );
	delete fileBuffer;
}


//---------------------------------------------------------------------------
Texture::Texture( unsigned char* imageData, Vec2i& size, int numComponents )
	: m_size( size )
	, m_textureID( 0 )
{
	GenerateOpenGLTexture( imageData, numComponents, 0 );
}


Texture::~Texture()
{
	
}


//---------------------------------------------------------------------------
bool Texture::LoadFileIfAble( const std::string& imageFilePath )
{
	int numComponents = 0; // Filled in for us to indicate how many color/alpha components the image had (e.g. 3=RGB, 4=RGBA)
	int numComponentsRequested = 0; // don't care; we support 3 (RGB) or 4 (RGBA)
	unsigned char* imageData = nullptr;

	imageData = stbi_load( imageFilePath.c_str(), &m_size.x, &m_size.y, &numComponents, numComponentsRequested );

	if ( !imageData ) return false;

	GenerateOpenGLTexture( imageData, numComponents, numComponentsRequested );

	stbi_image_free( imageData );
	return true;
}


//---------------------------------------------------------------------------
bool Texture::LoadFileIfAble( ZipFile* zipArchive, const char* imageFilePath )
{
	int numComponents = 0;
	int numComponentsRequested = 0;
	size_t fileBufferLength = 0;
	unsigned char* fileBuffer = nullptr;
	fileBuffer = reinterpret_cast< unsigned char* > ( zipArchive->LoadFileToHeap( imageFilePath, fileBufferLength ) );

	if ( !fileBuffer ) return false;

	unsigned char* imageData = nullptr;
	
	imageData = stbi_load_from_memory( fileBuffer, ( int ) fileBufferLength, &m_size.x, &m_size.y, &numComponents, numComponentsRequested );

	GenerateOpenGLTexture( imageData, numComponents, numComponentsRequested );

	stbi_image_free( imageData );
	delete fileBuffer;
	return true;
}


//---------------------------------------------------------------------------
// Returns a pointer to the already-loaded texture of a given image file,
//	or nullptr if no such texture/image has been loaded.
//
STATIC Texture* Texture::GetTextureByName( const std::string& name )
{
	// Todo: you write this
	Texture* textureToReturn = nullptr;

	if ( s_textureRegistry.find( name ) != s_textureRegistry.end() )
	{
		textureToReturn = s_textureRegistry[ name ];
	}

	return textureToReturn;
}


//---------------------------------------------------------------------------
// Finds the named Texture among the registry of those already loaded; if
//	found, returns that Texture*.  If not, attempts to load that texture,
//	and returns a Texture* just created (or nullptr if unable to load file).
//
STATIC Texture* Texture::CreateOrGetTexture( const std::string& imageFilePath )
{
	Texture* textureToReturn = nullptr;

	if ( s_textureRegistry.find( imageFilePath ) != s_textureRegistry.end() )
	{
		textureToReturn = s_textureRegistry[ imageFilePath ];
	}
	else if ( imageFilePath == "defaultDiffuse" )
	{
		GenerateDefaultDiffuseTexture();
		textureToReturn = s_textureRegistry[ imageFilePath ];
	}
	else if ( imageFilePath == "defaultNormal" )
	{
		GenerateDefaultNormalTexture();
		textureToReturn = s_textureRegistry[ imageFilePath ];
	}
	else if ( imageFilePath == "defaultSpecular" )
	{
		GenerateDefaultSpecGlossEmissiveTexture();
		textureToReturn = s_textureRegistry[ imageFilePath ];
	}
	else
	{
		textureToReturn = new Texture();
		if ( textureToReturn->LoadFileIfAble( imageFilePath ) )
		{
			s_textureRegistry.insert( std::make_pair( imageFilePath, textureToReturn ) );
		}
		else
		{
			delete textureToReturn;
			textureToReturn = nullptr;
		}
	}

	return textureToReturn;
}


//---------------------------------------------------------------------------
STATIC Texture* Texture::CreateOrGetTexture( ZipFile* zipArchive, const std::string& imageFilePath )
{
	Texture* textureToReturn = nullptr;

	if ( s_textureRegistry.find( imageFilePath ) != s_textureRegistry.end() )
	{
		textureToReturn = s_textureRegistry[ imageFilePath ];
	}
	else
	{
		textureToReturn = new Texture();
		if ( textureToReturn->LoadFileIfAble( zipArchive, imageFilePath.c_str() ) )
		{
			s_textureRegistry.insert( std::make_pair( imageFilePath, textureToReturn ) );
		}
		else
		{
			delete textureToReturn;
			textureToReturn = nullptr;
		}
	}

	return textureToReturn;
}


//---------------------------------------------------------------------------
STATIC void Texture::FreeTextureRegistryMemory()
{
	std::map< std::string, Texture* >::iterator registryIterator;
	for ( registryIterator = s_textureRegistry.begin(); registryIterator != s_textureRegistry.end(); )
	{
		g_theRenderer->VennDeleteTextures( 1, &registryIterator->second->m_textureID );
		delete registryIterator->second;
		registryIterator = s_textureRegistry.erase( registryIterator++ );
	}
}


//---------------------------------------------------------------------------
void Texture::GenerateOpenGLTexture( unsigned char* imageData, int numComponents, int numComponentsRequested )
{
	UNUSED( numComponentsRequested );

	// Tell OpenGL that our pixel data is single-byte aligned
	g_theRenderer->VennPixelStorei( V_UNPACK_ALIGNMENT, 1 );

	// Ask OpenGL for an unused texName (ID number) to use for this texture
	g_theRenderer->VennGenTextures( 1, &m_textureID );

	// Tell OpenGL to bind (set) this as the currently active texture
	g_theRenderer->VennBindTexture( V_TEXTURE_2D, m_textureID );

	// level of mipmap
	g_theRenderer->VennTexParameteri( V_TEXTURE_2D, V_TEXTURE_MAX_LEVEL, 5 );

	// Set texture clamp vs. wrap (repeat)
	g_theRenderer->VennTexParameteri( V_TEXTURE_2D, V_TEXTURE_WRAP_S, V_CLAMP_TO_EDGE ); // one of: V_CLAMP, V_REPEAT, V_MIRRORED_REPEAT, V_MIRROR_CLAMP_TO_EDGE, ...
	g_theRenderer->VennTexParameteri( V_TEXTURE_2D, V_TEXTURE_WRAP_T, V_CLAMP_TO_EDGE ); // one of: V_CLAMP, V_REPEAT, V_MIRRORED_REPEAT, V_MIRROR_CLAMP_TO_EDGE, ...

	// Set magnification (texel > pixel) and minification (texel < pixel) filters
	g_theRenderer->VennTexParameteri( V_TEXTURE_2D, V_TEXTURE_MAG_FILTER, V_LINEAR ); // one of: V_NEAREST, V_LINEAR
	g_theRenderer->VennTexParameteri( V_TEXTURE_2D, V_TEXTURE_MIN_FILTER, V_LINEAR_MIPMAP_NEAREST ); // one of: V_NEAREST, V_LINEAR, V_NEAREST_MIPMAP_NEAREST, V_NEAREST_MIPMAP_LINEAR, V_LINEAR_MIPMAP_NEAREST, V_LINEAR_MIPMAP_LINEAR

	VennEnum bufferFormat = V_RGBA; // the format our source pixel data is currently in; any of: V_RGB, V_RGBA, V_LUMINANCE, V_LUMINANCE_ALPHA, ...
	if( numComponents == 3 )
	{
		bufferFormat = V_RGB;
	}

	// Todo: What happens if numComponents is neither 3 nor 4?

	VennEnum internalFormat = bufferFormat; // the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL

	g_theRenderer->VennTexImage2D(			// Upload this pixel data to our new OpenGL texture
		V_TEXTURE_2D,		// Creating this as a 2d texture
		0,					// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
		internalFormat,		// Type of texel format we want OpenGL to use for this texture internally on the video card
		m_size.x,			// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
		m_size.y,			// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
		0,					// Border size, in texels (must be 0 or 1)
		bufferFormat,		// Pixel format describing the composition of the pixel data in buffer
		V_UNSIGNED_BYTE,	// Pixel color components are unsigned bytes (one byte per color/alpha channel)
		imageData );		// Location of the actual pixel data bytes/buffer

	g_theRenderer->VennHint( V_GENERATE_MIPMAP_HINT, V_NICEST );
	g_theRenderer->VennGenerateMipmap( V_TEXTURE_2D );
}


//---------------------------------------------------------------------------
STATIC void Texture::GenerateDefaultTextures()
{
	GenerateDefaultDiffuseTexture();
	GenerateDefaultNormalTexture();
	GenerateDefaultSpecGlossEmissiveTexture();
}


//---------------------------------------------------------------------------
void Texture::GenerateDefaultDiffuseTexture()
{
	Vec2i size( 1, 1 );
	std::string name( "defaultDiffuse" );
	unsigned char* imageData = new unsigned char[ 3 ];
	imageData[ 0 ] = 255;
	imageData[ 1 ] = 255;
	imageData[ 2 ] = 255;

	std::map< std::string, Texture* >::iterator registryIterator;
	if ( s_textureRegistry.find( name ) == s_textureRegistry.end() )
	{
		s_textureRegistry.insert( std::make_pair( name, new Texture( imageData, size, 3 ) ) );
	}
	delete imageData;
}


//---------------------------------------------------------------------------
void Texture::GenerateDefaultNormalTexture()
{
	Vec2i size( 1, 1 );
	std::string name( "defaultNormal" );
	unsigned char* imageData = new unsigned char[ 3 ];
	imageData[ 0 ] = 128;
	imageData[ 1 ] = 128;
	imageData[ 2 ] = 255;

	std::map< std::string, Texture* >::iterator registryIterator;
	if ( s_textureRegistry.find( name ) == s_textureRegistry.end() )
	{
		s_textureRegistry.insert( std::make_pair( name, new Texture( imageData, size, 3 ) ) );
	}
	delete imageData;
}


//---------------------------------------------------------------------------
void Texture::GenerateDefaultSpecGlossEmissiveTexture()
{
	Vec2i size( 1, 1 );
	std::string name( "defaultSpecular" );
	unsigned char* imageData = new unsigned char[ 3 ];
	imageData[ 0 ] = 127;
	imageData[ 1 ] = 127;
	imageData[ 2 ] = 0;

	std::map< std::string, Texture* >::iterator registryIterator;
	if ( s_textureRegistry.find( name ) == s_textureRegistry.end() )
	{
		s_textureRegistry.insert( std::make_pair( name, new Texture( imageData, size, 3 ) ) );
	}
	delete imageData;
}