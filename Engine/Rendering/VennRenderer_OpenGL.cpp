//---------------------------------------------------------------------------
// TheRenderer_OpenGL.cpp
//---------------------------------------------------------------------------

#include "VennRenderer.hpp"
#include "DebugDraw.hpp"
#include "Engine/Core/EngineCommon.hpp"


VennRenderer* g_theRenderer = nullptr;

// Globals "function pointer" variables - Squirrel Eiserloh
PFNGLGENBUFFERSPROC					glGenBuffers				= nullptr;
PFNGLBINDBUFFERPROC					glBindBuffer				= nullptr;
PFNGLBUFFERDATAPROC					glBufferData				= nullptr;
PFNGLGENERATEMIPMAPPROC				glGenerateMipmap			= nullptr;
PFNGLDELETEBUFFERSPROC				glDeleteBuffers				= nullptr;
PFNGLCREATESHADERPROC				glCreateShader				= nullptr;
PFNGLSHADERSOURCEPROC				glShaderSource				= nullptr;
PFNGLCOMPILESHADERPROC				glCompileShader				= nullptr;
PFNGLATTACHSHADERPROC				glAttachShader				= nullptr;
PFNGLDETACHSHADERPROC				glDetachShader				= nullptr;
PFNGLDELETESHADERPROC				glDeleteShader				= nullptr;
PFNGLDELETEPROGRAMPROC				glDeleteProgram				= nullptr;
PFNGLCREATEPROGRAMPROC				glCreateProgram				= nullptr;
PFNGLLINKPROGRAMPROC				glLinkProgram				= nullptr;
PFNGLUSEPROGRAMPROC					glUseProgram				= nullptr;
PFNGLUNIFORM1FPROC					glUniform1f					= nullptr;
PFNGLUNIFORM1FVPROC					glUniform1fv				= nullptr;
PFNGLUNIFORM1IPROC					glUniform1i					= nullptr;
PFNGLUNIFORM3FPROC					glUniform3f					= nullptr;
PFNGLUNIFORM3FVPROC					glUniform3fv				= nullptr;
PFNGLUNIFORM4FPROC					glUniform4f					= nullptr;
PFNGLUNIFORM4FVPROC					glUniform4fv				= nullptr;
PFNGLUNIFORMMATRIX4FVPROC			glUniformMatrix4fv			= nullptr;
PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation		= nullptr;
PFNGLGETSHADERIVPROC				glGetShaderiv				= nullptr;
PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog			= nullptr;
PFNGLGETPROGRAMIVPROC				glGetProgramiv				= nullptr;
PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog			= nullptr;
PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation			= nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC	glEnableVertexAttribArray	= nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC	glDisableVertexAttribArray	= nullptr;
PFNGLVERTEXATTRIBPOINTERPROC		glVertexAttribPointer		= nullptr;
PFNGLVERTEXATTRIB4FPROC				glVertexAttrib4f			= nullptr;
PFNGLVERTEXATTRIB3FPROC				glVertexAttrib3f			= nullptr;
PFNGLVERTEXATTRIB2FPROC				glVertexAttrib2f			= nullptr;
PFNGLACTIVETEXTUREPROC				glActiveTexture				= nullptr;
PFNGLGENFRAMEBUFFERSPROC			glGenFramebuffers			= nullptr;
PFNGLDELETEFRAMEBUFFERSPROC			glDeleteFramebuffers		= nullptr;
PFNGLBINDFRAMEBUFFERPROC			glBindFramebuffer			= nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC		glFramebufferTexture2D		= nullptr;


//---------------------------------------------------------------------------
VennRenderer::VennRenderer()
{
	Initialize();
}


//---------------------------------------------------------------------------
VennRenderer::~VennRenderer()
{
	m_matrixStack.clear();
	std::vector< Matrix4 >().swap( m_matrixStack );
}


//---------------------------------------------------------------------------
void VennRenderer::Initialize()
{
	m_matrixStack.push_back( Matrix4() );
	InitializeAdvancedRenderingFunctions();
}


//---------------------------------------------------------------------------
void VennRenderer::UpdateDebugDrawList()
{
	for ( unsigned int index = 0; index < g_drawObjects.size(); ++index )
	{
		g_drawObjects[ index ]->Update();

		if ( g_drawObjects[ index ]->IsDead() )
		{
			delete g_drawObjects[ index ];
			g_drawObjects.erase( g_drawObjects.begin() + index );
			--index;
		}
	}
}


//---------------------------------------------------------------------------
// Code provided by Squirrel Eiserloh
void VennRenderer::InitializeAdvancedRenderingFunctions()
{
	glGenBuffers				= (PFNGLGENBUFFERSPROC)					wglGetProcAddress( "glGenBuffers" );
	glBindBuffer				= (PFNGLBINDBUFFERPROC)					wglGetProcAddress( "glBindBuffer" );
	glBufferData				= (PFNGLBUFFERDATAPROC)					wglGetProcAddress( "glBufferData" );
	glGenerateMipmap			= (PFNGLGENERATEMIPMAPPROC)				wglGetProcAddress( "glGenerateMipmap" );
	glDeleteBuffers				= (PFNGLDELETEBUFFERSPROC)				wglGetProcAddress( "glDeleteBuffers" );
	glCreateShader				= (PFNGLCREATESHADERPROC)				wglGetProcAddress( "glCreateShader" );
	glShaderSource				= (PFNGLSHADERSOURCEPROC)				wglGetProcAddress( "glShaderSource" );
	glCompileShader				= (PFNGLCOMPILESHADERPROC)				wglGetProcAddress( "glCompileShader" );
	glAttachShader				= (PFNGLATTACHSHADERPROC)				wglGetProcAddress( "glAttachShader" );
	glDetachShader				= (PFNGLDETACHSHADERPROC)				wglGetProcAddress( "glDetachShader" );
	glDeleteShader				= (PFNGLDELETESHADERPROC)				wglGetProcAddress( "glDeleteShader" );
	glDeleteProgram				= (PFNGLDELETEPROGRAMPROC)				wglGetProcAddress( "glDeleteProgram" );
	glCreateProgram				= (PFNGLCREATEPROGRAMPROC)				wglGetProcAddress( "glCreateProgram" );
	glLinkProgram				= (PFNGLLINKPROGRAMPROC)				wglGetProcAddress( "glLinkProgram" );
	glUseProgram				= (PFNGLUSEPROGRAMPROC)					wglGetProcAddress( "glUseProgram" );
	glUniform1i					= (PFNGLUNIFORM1IPROC)					wglGetProcAddress( "glUniform1i" );
	glUniform1f					= (PFNGLUNIFORM1FPROC)					wglGetProcAddress( "glUniform1f" );
	glUniform1fv				= (PFNGLUNIFORM1FVPROC)					wglGetProcAddress( "glUniform1fv" );
	glUniform3f					= (PFNGLUNIFORM3FPROC)					wglGetProcAddress( "glUniform3f" );
	glUniform3fv				= (PFNGLUNIFORM3FVPROC)					wglGetProcAddress( "glUniform3fv" );
	glUniform4f					= (PFNGLUNIFORM4FPROC)					wglGetProcAddress( "glUniform4f" );
	glUniform4fv				= (PFNGLUNIFORM4FVPROC)					wglGetProcAddress( "glUniform4fv" );
	glUniformMatrix4fv			= (PFNGLUNIFORMMATRIX4FVPROC)			wglGetProcAddress( "glUniformMatrix4fv" );
	glGetUniformLocation		= (PFNGLGETUNIFORMLOCATIONPROC)			wglGetProcAddress( "glGetUniformLocation" );
	glGetShaderiv				= (PFNGLGETSHADERIVPROC)				wglGetProcAddress( "glGetShaderiv" );
	glGetShaderInfoLog			= (PFNGLGETSHADERINFOLOGPROC)			wglGetProcAddress( "glGetShaderInfoLog" );
	glGetProgramiv				= (PFNGLGETPROGRAMIVPROC)				wglGetProcAddress( "glGetProgramiv" );
	glGetProgramInfoLog			= (PFNGLGETPROGRAMINFOLOGPROC)			wglGetProcAddress( "glGetProgramInfoLog" );
	glGetAttribLocation			= (PFNGLGETATTRIBLOCATIONPROC)			wglGetProcAddress( "glGetAttribLocation" );
	glEnableVertexAttribArray	= (PFNGLENABLEVERTEXATTRIBARRAYPROC)	wglGetProcAddress( "glEnableVertexAttribArray" );
	glDisableVertexAttribArray	= (PFNGLDISABLEVERTEXATTRIBARRAYPROC)	wglGetProcAddress( "glDisableVertexAttribArray" );
	glVertexAttribPointer		= (PFNGLVERTEXATTRIBPOINTERPROC)		wglGetProcAddress( "glVertexAttribPointer" );
	glVertexAttrib4f			= (PFNGLVERTEXATTRIB4FPROC)				wglGetProcAddress( "glVertexAttrib4f" );
	glVertexAttrib3f			= (PFNGLVERTEXATTRIB3FPROC)				wglGetProcAddress( "glVertexAttrib3f" );
	glVertexAttrib2f			= (PFNGLVERTEXATTRIB2FPROC)				wglGetProcAddress( "glVertexAttrib2f" );
	glActiveTexture				= (PFNGLACTIVETEXTUREPROC)				wglGetProcAddress( "glActiveTexture" );
	glGenFramebuffers			= (PFNGLGENFRAMEBUFFERSPROC)			wglGetProcAddress( "glGenFramebuffers" );
	glDeleteFramebuffers		= (PFNGLDELETEFRAMEBUFFERSPROC)			wglGetProcAddress( "glDeleteFramebuffers" );
	glBindFramebuffer			= (PFNGLBINDFRAMEBUFFERPROC)			wglGetProcAddress( "glBindFramebuffer" );
	glFramebufferTexture2D		= (PFNGLFRAMEBUFFERTEXTURE2DPROC)		wglGetProcAddress( "glFramebufferTexture2D" );
}


//---------------------------------------------------------------------------
void VennRenderer::VennBegin( VennEnum mode )
{
	glBegin( mode );
}


//---------------------------------------------------------------------------
void VennRenderer::VennEnd()
{
	glEnd();
}


//---------------------------------------------------------------------------
void VennRenderer::VennEnable( VennEnum cap )
{
	glEnable( cap );
}


//---------------------------------------------------------------------------
void VennRenderer::VennDisable( VennEnum cap )
{
	glDisable( cap );
}


//---------------------------------------------------------------------------
void VennRenderer::VennPushMatrix()
{
	glPushMatrix();
}


//---------------------------------------------------------------------------
void VennRenderer::VennPopMatrix()
{
	glPopMatrix();
}


//---------------------------------------------------------------------------
void VennRenderer::VennLoadMatrixf( const float* m )
{
	glLoadMatrixf( m );	
}


//---------------------------------------------------------------------------
void VennRenderer::VennClearColor( float red, float green, float blue, float alpha )
{
	glClearColor( red, green, blue, alpha );
}


//---------------------------------------------------------------------------
void VennRenderer::VennClearColor( const Vec4f& colorValue )
{
	glClearColor( colorValue.x, colorValue.y, colorValue.z, colorValue.w );
}


//---------------------------------------------------------------------------
void VennRenderer::VennClearColor( const Rgba& colorValue )
{
	glClearColor( colorValue.r, colorValue.g, colorValue.b, colorValue.a );
}


//---------------------------------------------------------------------------
void VennRenderer::VennClearDepth( double depth )
{
	glClearDepth( depth );
}


//---------------------------------------------------------------------------
void VennRenderer::VennDepthMask( VennEnum flag )
{
	glDepthMask( ( GLboolean ) flag );
}


//---------------------------------------------------------------------------
void VennRenderer::VennClear(  unsigned int mask )
{
	glClear( mask );
}


//---------------------------------------------------------------------------
void VennRenderer::VennLineWidth( float width )
{
	glLineWidth( width );
}


//---------------------------------------------------------------------------
void VennRenderer::VennPointSize( float size )
{
	glPointSize( size );
}


//---------------------------------------------------------------------------
void VennRenderer::VennBlendFunc( VennEnum sfactor, VennEnum dfactor )
{
	glBlendFunc( sfactor, dfactor );
}


//---------------------------------------------------------------------------
void VennRenderer::VennPolygonMode( VennEnum face, VennEnum mode )
{
	glPolygonMode( face, mode );
}


//---------------------------------------------------------------------------
void VennRenderer::VennTranslatef( float x, float y, float z )
{
	glTranslatef( x, y, z );
}


//---------------------------------------------------------------------------
void VennRenderer::VennRotatef( float angle, float x, float y, float z )
{
	glRotatef( angle, x, y, z );
}


//---------------------------------------------------------------------------
void VennRenderer::VennScalef( float x, float y, float z )
{
	glScalef( x, y, z );
}


//---------------------------------------------------------------------------
void VennRenderer::VennVertex2f( float x, float y )
{
	glVertex2f( x, y );
}


//---------------------------------------------------------------------------
void VennRenderer::VennVertex3f( float x, float y, float z )
{
	glVertex3f( x, y, z );
}


//---------------------------------------------------------------------------
void VennRenderer::VennColor3f( float red, float green, float blue )
{
	glColor3f( red, green, blue );
}


//---------------------------------------------------------------------------
void VennRenderer::VennTexCoord2f( float s, float t )
{
	glTexCoord2f( s, t );
}


//---------------------------------------------------------------------------
void VennRenderer::VennPixelStorei( VennEnum pname, int param )
{
	glPixelStorei( pname, param );
}


//---------------------------------------------------------------------------
void VennRenderer::VennGenTextures( int n, unsigned int* textures )
{
	glGenTextures( n, textures );
}


//---------------------------------------------------------------------------
void VennRenderer::VennDeleteTextures( int n, unsigned int* textures )
{
	glDeleteTextures( n, textures );
}


//---------------------------------------------------------------------------
void VennRenderer::VennActiveTexture( VennEnum texture )
{
	glActiveTexture( texture );
}


//---------------------------------------------------------------------------
void VennRenderer::VennBindTexture( VennEnum target, int texture )
{
	glBindTexture( target, texture );
}


//---------------------------------------------------------------------------
void VennRenderer::VennTexParameteri( VennEnum target, VennEnum pname, int param )
{
	glTexParameteri( target, pname, param );	
}


//---------------------------------------------------------------------------
void VennRenderer::VennTexImage2D( VennEnum target, int level, int internalFormat, int width, int height, int border, VennEnum format, VennEnum type, const void* pixels )
{
	glTexImage2D( target, level, internalFormat, width, height, border, format, type, pixels );
}


//---------------------------------------------------------------------------
void VennRenderer::VennGenBuffers( int n, unsigned int* buffers )
{
	glGenBuffers( n, buffers );
}


//---------------------------------------------------------------------------
void VennRenderer::VennBindBuffer( VennEnum target, unsigned int buffer )
{
	glBindBuffer( target, buffer );
}


//---------------------------------------------------------------------------
void VennRenderer::VennBufferData( VennEnum target, int size, const void* data, VennEnum usage )
{
	glBufferData( target, size, data, usage );
}


//---------------------------------------------------------------------------
void VennRenderer::VennGenerateMipmap( VennEnum target )
{
	glGenerateMipmap( target );
}


//---------------------------------------------------------------------------
void VennRenderer::VennDeleteBuffers( int n, const unsigned int* buffers )
{
	glDeleteBuffers( n, buffers );
}


//---------------------------------------------------------------------------
void VennRenderer::VennVertexPointer( int size, VennEnum type, int stride, const void* pointer )
{
	glVertexPointer( size, type, stride, pointer );
}


//---------------------------------------------------------------------------
void VennRenderer::VennColorPointer( int size, VennEnum type, int stride, const void* pointer )
{
	glColorPointer( size, type, stride, pointer );
}


//---------------------------------------------------------------------------
void VennRenderer::VennTexCoordPointer( int size, VennEnum type, int stride, const void* pointer )
{
	glTexCoordPointer( size, type, stride, pointer );
}


//---------------------------------------------------------------------------
void VennRenderer::VennDrawArrays( VennEnum mode, int first, int count )
{
	glDrawArrays( mode, first, count );
}


//---------------------------------------------------------------------------
void VennRenderer::VennDrawElements( VennEnum mode, int count, VennEnum type, const void* indices )
{
	glDrawElements( mode, count, type, indices );
}


//---------------------------------------------------------------------------
void VennRenderer::VennOrtho( double left, double right, double bottom, double top, double zNear, double zFar )
{
	glOrtho( left, right, bottom, top, zNear, zFar );
}


//---------------------------------------------------------------------------
void VennRenderer::VennLoadIdentity()
{
	glLoadIdentity();
}


//---------------------------------------------------------------------------
void VennRenderer::VennPerspective( double fovy, double aspect, double zNear, double zFar )
{
	gluPerspective( fovy, aspect, zNear, zFar );
}


//---------------------------------------------------------------------------
void VennRenderer::VennHint( VennEnum target, VennEnum mode )
{
	glHint( target, mode );
}


//---------------------------------------------------------------------------
void VennRenderer::VennEnableClientState( VennEnum array )
{
	glEnableClientState( array );
}


//---------------------------------------------------------------------------
void VennRenderer::VennDisableClientState( VennEnum array )
{
	glDisableClientState( array );
}


//---------------------------------------------------------------------------
int VennRenderer::VennCreateShader( VennEnum type )
{
	return glCreateShader( type );	
}


//---------------------------------------------------------------------------
void VennRenderer::VennShaderSource( unsigned int shader, int count, const char** string, int* length )
{
	glShaderSource( shader, count, string, length );
}


//---------------------------------------------------------------------------
void VennRenderer::VennCompileShader( unsigned int shader )
{
	glCompileShader( shader );
}


//---------------------------------------------------------------------------
void VennRenderer::VennAttachShader( unsigned int program, unsigned int shader )
{
	glAttachShader( program, shader );
}


//---------------------------------------------------------------------------
void VennRenderer::VennDetachShader( unsigned int program, unsigned int shader )
{
	glDetachShader( program, shader );
}


//---------------------------------------------------------------------------
void VennRenderer::VennDeleteShader( unsigned int shader )
{
	glDeleteShader( shader );
}


//---------------------------------------------------------------------------
void VennRenderer::VennDeleteProgram( unsigned int program )
{
	glDeleteProgram( program );
}


//---------------------------------------------------------------------------
unsigned int VennRenderer::VennCreateProgram()
{
	return glCreateProgram();
}


//---------------------------------------------------------------------------
void VennRenderer::VennLinkProgram( unsigned int program )
{
	glLinkProgram( program );
}


//---------------------------------------------------------------------------
void VennRenderer::VennUseProgram( unsigned int program )
{
	glUseProgram( program );
}


//---------------------------------------------------------------------------
void VennRenderer::VennUniform1f( int location, float v0 )
{
	glUniform1f( location, v0 );
}


//---------------------------------------------------------------------------
void VennRenderer::VennUniform1fv( int location, int count, const float* value )
{
	glUniform1fv( location, count, value );
}


//---------------------------------------------------------------------------
void VennRenderer::VennUniform1i( int location, int v0 )
{
	glUniform1i( location, v0 );
}


//---------------------------------------------------------------------------
void VennRenderer::VennUniform3f( int location, float v0, float v1, float v2 )
{
	glUniform3f( location, v0, v1, v2 );
}


//---------------------------------------------------------------------------
void VennRenderer::VennUniform3fv( int location, int count, const float* value )
{
	glUniform3fv( location, count, value );
}


//---------------------------------------------------------------------------
void VennRenderer::VennUniform4fv( int location, int count, const float* value )
{
	glUniform4fv( location, count, value );
}


//---------------------------------------------------------------------------
void VennRenderer::VennUniform4f( int location, float v0, float v1, float v2, float v3 )
{
	glUniform4f( location, v0, v1, v2, v3 );
}


//---------------------------------------------------------------------------
void VennRenderer::VennUniformMatrix4fv( int location, int count, bool transpose, float* value )
{
	glUniformMatrix4fv( location, count, transpose, value );
}


//---------------------------------------------------------------------------
int VennRenderer::VennGetUniformLocation( unsigned int program, const char* name )
{
	int loc = glGetUniformLocation( program, name );
	return loc;
}


//---------------------------------------------------------------------------
void VennRenderer::VennGetShaderiv( unsigned int shader, VennEnum name, int* params )
{
	glGetShaderiv( shader, name, params );
}


//---------------------------------------------------------------------------
void VennRenderer::VennGetShaderInfoLog( unsigned int shader, int bufSize, int* length, char* infoLog )
{
	glGetShaderInfoLog( shader, bufSize, length, infoLog );
}


//---------------------------------------------------------------------------
void VennRenderer::VennGetProgramiv( unsigned int program, VennEnum pname, int* params )
{
	glGetProgramiv( program, pname, params );
}


//---------------------------------------------------------------------------
void VennRenderer::VennGetProgramInfoLog( unsigned int program, int bufSize, int* length, char* infoLog )
{
	glGetProgramInfoLog( program, bufSize, length, infoLog );
}


//---------------------------------------------------------------------------
const unsigned char* VennRenderer::VennGetString( VennEnum name )
{
	return glGetString( name );
}


//---------------------------------------------------------------------------
int VennRenderer::VennGetAttribLocation( unsigned int program, const char* name )
{
	return glGetAttribLocation( program, name );
}


//---------------------------------------------------------------------------
void VennRenderer::VennEnableVertexAttribArray( unsigned int index )
{
	glEnableVertexAttribArray( index );
}


//---------------------------------------------------------------------------
void VennRenderer::VennDisableVertexAttribArray( unsigned int index )
{
	glDisableVertexAttribArray( index );
}


//---------------------------------------------------------------------------
void VennRenderer::VennVertexAttribPointer( unsigned int index, int size, VennEnum type, bool normalized, int stride, const void* pointer )
{
	glVertexAttribPointer( index, size, type, normalized, stride, pointer );
}


//---------------------------------------------------------------------------
void VennRenderer::VennVertexAttrib4f( unsigned int index, float x, float y, float z, float w )
{
	glVertexAttrib4f( index, x, y, z, w );
}


//---------------------------------------------------------------------------
void VennRenderer::VennVertexAttrib3f( unsigned int index, float x, float y, float z )
{
	glVertexAttrib3f( index, x, y, z );
}


//---------------------------------------------------------------------------
void VennRenderer::VennVertexAttrib2f( unsigned int index, float x, float y )
{
	glVertexAttrib2f( index, x, y );
}


//---------------------------------------------------------------------------
void VennRenderer::VennGenFramebuffers( int n, unsigned int* framebuffers )
{
	glGenFramebuffers( n, framebuffers );
}


//---------------------------------------------------------------------------
void VennRenderer::VennDeleteFramebuffers( int n, const unsigned int* framebuffers )
{
	glDeleteFramebuffers( n, framebuffers );
}


//---------------------------------------------------------------------------
void VennRenderer::VennBindFramebuffer( VennEnum target, unsigned int framebuffer )
{
	glBindFramebuffer( target, framebuffer );
}


//---------------------------------------------------------------------------
void VennRenderer::VennFramebufferTexture2D( VennEnum target, VennEnum attachment, VennEnum textarget, unsigned int texture, int level )
{
	glFramebufferTexture2D( target, attachment, textarget, texture, level );
}