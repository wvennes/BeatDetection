//---------------------------------------------------------------------------
// TheRenderer.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_THERENDERER_
#define _INCLUDED_THERENDERER_

//#ifdef USING_OPENGL

#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "glext.h"
#include "Engine/Math/Rgba.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/Matrix4.hpp"

class DebugDrawObject;

extern enum VennEnum
{
	V_ZERO = GL_ZERO,
	V_FALSE = GL_FALSE,
	V_TRUE = GL_TRUE,
	V_COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT,
	V_DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT,
	V_POINTS = GL_POINTS,
	V_LINES = GL_LINES,
	V_LINE_LOOP = GL_LINE_LOOP,
	V_FRONT_AND_BACK = GL_FRONT_AND_BACK,
	V_LINE = GL_LINE,
	V_TRIANGLES = GL_TRIANGLES,
	V_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
	V_TRIANGLE_FAN = GL_TRIANGLE_FAN,
	V_QUADS = GL_QUADS,
	V_QUAD_STRIP = GL_QUAD_STRIP,
	V_ONE = GL_ONE,
	V_SRC_ALPHA = GL_SRC_ALPHA,
	V_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
	V_ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
	V_LINE_SMOOTH = GL_LINE_SMOOTH,
	V_CULL_FACE = GL_CULL_FACE,
	V_DEPTH_TEST = GL_DEPTH_TEST,
	V_BLEND = GL_BLEND,
	V_LINE_SMOOTH_HINT = GL_LINE_SMOOTH_HINT,
	V_UNPACK_ALIGNMENT = GL_UNPACK_ALIGNMENT,
	V_TEXTURE_2D = GL_TEXTURE_2D,
	V_DONT_CARE = GL_DONT_CARE,
	V_NICEST = GL_NICEST,
	V_UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
	V_UNSIGNED_INT = GL_UNSIGNED_INT,
	V_FLOAT = GL_FLOAT,
	V_RGB = GL_RGB,
	V_RGBA = GL_RGBA,
	V_NEAREST = GL_NEAREST,
	V_LINEAR = GL_LINEAR,
	V_NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	V_LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	V_NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	V_LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
	V_TEXTURE_MAG_FILTER = GL_TEXTURE_MAG_FILTER,
	V_TEXTURE_MIN_FILTER = GL_TEXTURE_MIN_FILTER,
	V_TEXTURE_WRAP_S = GL_TEXTURE_WRAP_S,
	V_TEXTURE_WRAP_T = GL_TEXTURE_WRAP_T,
	V_CLAMP = GL_CLAMP,
	V_VERTEX_ARRAY = GL_VERTEX_ARRAY,
	V_COLOR_ARRAY = GL_COLOR_ARRAY,
	V_TEXTURE_COORD_ARRAY = GL_TEXTURE_COORD_ARRAY,
	V_GENERATE_MIPMAP_HINT = GL_GENERATE_MIPMAP_HINT,
	V_TEXTURE_MAX_LEVEL = GL_TEXTURE_MAX_LEVEL,
	V_ARRAY_BUFFER = GL_ARRAY_BUFFER,
	V_ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
	V_STATIC_DRAW = GL_STATIC_DRAW,
	V_VERTEX_SHADER = GL_VERTEX_SHADER,
	V_FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
	V_COMPILE_STATUS = GL_COMPILE_STATUS,
	V_LINK_STATUS = GL_LINK_STATUS,
	V_INFO_LOG_LENGTH = GL_INFO_LOG_LENGTH,
	V_VERSION = GL_VERSION,
	V_SHADING_LANGUAGE_VERSION = GL_SHADING_LANGUAGE_VERSION,
	V_TEXTURE0 = GL_TEXTURE0,
	V_TEXTURE1 = GL_TEXTURE1,
	V_TEXTURE2 = GL_TEXTURE2,
	V_TEXTURE3 = GL_TEXTURE3,
	V_TEXTURE4 = GL_TEXTURE4,
	V_TEXTURE5 = GL_TEXTURE5,
	V_TEXTURE6 = GL_TEXTURE6,
	V_FRAMEBUFFER = GL_FRAMEBUFFER,
	V_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
	V_COLOR_ATTACHMENT0 = GL_COLOR_ATTACHMENT0,
	V_DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
	V_DEPTH_COMPONENT = GL_DEPTH_COMPONENT
};

//#endif

/////////////////////////////////////////////////////////////////////////////
class VennRenderer
{

public:
	VennRenderer();
	~VennRenderer();

	void UpdateDebugDrawList();

	void VennBegin( VennEnum mode );
	void VennEnd();

	void VennEnable( VennEnum cap );
	void VennDisable( VennEnum cap );
	void VennPushMatrix();
	void VennPopMatrix();
	void VennLoadMatrixf( const float* m );
	void VennClearColor( float red, float green, float blue, float alpha );
	void VennClearColor( const Vec4f& colorValue );
	void VennClearColor( const Rgba& colorValue );
	void VennClearDepth( double depth );
	void VennDepthMask( VennEnum flag );
	void VennClear( unsigned int mask );

	void VennLineWidth( float width );
	void VennPointSize( float size );
	void VennBlendFunc( VennEnum sfactor, VennEnum dfactor );
	void VennPolygonMode( VennEnum face, VennEnum mode );

	void VennVertex2f( float x, float y );
	void VennVertex3f( float x, float y, float z );
	void VennColor3f( float red, float green, float blue );
	void VennTexCoord2f( float s, float t );
	void VennTranslatef( float x, float y, float z );
	void VennRotatef( float angle, float x, float y, float z );
	void VennScalef( float x, float y, float z );
	void VennPixelStorei( VennEnum pname, int param );

	void VennGenTextures( int n, unsigned int* textures );
	void VennDeleteTextures( int n, unsigned int* textures );
	void VennActiveTexture( VennEnum texture );
	void VennBindTexture( VennEnum target, int texture );
	void VennTexParameteri( VennEnum target, VennEnum pname, int param );
	void VennTexImage2D( VennEnum target, int level, int internalFormat, int width, int height, int border, VennEnum format, VennEnum type, const void* pixels );
	
	void VennGenBuffers( int n, unsigned int* buffers );
	void VennBindBuffer( VennEnum target, unsigned int buffer );
	void VennBufferData( VennEnum target, int size, const void* data, VennEnum usage );
	void VennGenerateMipmap( VennEnum target );
	void VennDeleteBuffers( int n, const unsigned int* buffers );

	void VennVertexPointer( int size, VennEnum type, int stride, const void* pointer );
	void VennColorPointer( int size, VennEnum type, int stride, const void* pointer );
	void VennTexCoordPointer( int size, VennEnum type, int stride, const void* pointer );
	void VennDrawArrays( VennEnum mode, int first, int count );
	void VennDrawElements( VennEnum mode, int count, VennEnum type, const void* indices );

	void VennOrtho( double left, double right, double bottom, double top, double zNear, double zFar );
	void VennLoadIdentity();
	void VennPerspective( double fovy, double aspect, double zNear, double zFar );

	void VennHint( VennEnum target, VennEnum mode );
	void VennEnableClientState( VennEnum array );
	void VennDisableClientState( VennEnum array );

	int VennCreateShader( VennEnum type );
	void VennShaderSource( unsigned int shader, int count, const char** string, int* length );
	void VennCompileShader( unsigned int shader );
	void VennAttachShader( unsigned int program, unsigned int shader );
	void VennDetachShader( unsigned int program, unsigned int shader );
	void VennDeleteShader( unsigned int shader );
	void VennDeleteProgram( unsigned int program );
	unsigned int VennCreateProgram();
	void VennLinkProgram( unsigned int program );
	void VennUseProgram( unsigned int program );
	void VennUniform1f( int location, float v0 );
	void VennUniform1fv( int location, int count, const float* value );
	void VennUniform1i( int location, int v0 );
	void VennUniform3f( int location, float v0, float v1, float v2 );
	void VennUniform3fv( int location, int count, const float* value );
	void VennUniform4f( int location, float v0, float v1, float v2, float v3 );
	void VennUniform4fv( int location, int count, const float* value );
	void VennUniformMatrix4fv( int location, int count, bool transpose, float* value );
	int VennGetUniformLocation( unsigned int program, const char* name );

	void VennGetShaderiv( unsigned int shader, VennEnum name, int* params );
	void VennGetShaderInfoLog( unsigned int shader, int bufSize, int* length, char* infoLog );
	void VennGetProgramiv( unsigned int program, VennEnum pname, int* params );
	void VennGetProgramInfoLog( unsigned int program, int bufSize, int* length, char* infoLog );

	int VennGetAttribLocation( unsigned int program, const char* name );
	void VennEnableVertexAttribArray( unsigned int index );
	void VennDisableVertexAttribArray( unsigned int index );
	void VennVertexAttribPointer( unsigned int index, int size, VennEnum type, bool normalized, int stride, const void* pointer );
	void VennVertexAttrib4f( unsigned int index, float x, float y, float z, float w );
	void VennVertexAttrib3f( unsigned int index, float x, float y, float z );
	void VennVertexAttrib2f( unsigned int index, float x, float y );

	void VennGenFramebuffers( int n, unsigned int* framebuffers );
	void VennDeleteFramebuffers( int n, const unsigned int* framebuffers );
	void VennBindFramebuffer( VennEnum target, unsigned int framebuffer );
	void VennFramebufferTexture2D( VennEnum target, VennEnum attachment, VennEnum textarget, unsigned int texture, int level );

	const unsigned char* VennGetString( VennEnum name );

	// custom matrix stack operations
	void PushMatrix();
	void PopMatrix();
	void LoadIdentityMatrix();
	void ApplyRotation( const Degrees angle, const Vec3f& rotationAxis );
	void ApplyTranslation( const Vec3f& translation );
	void ApplyScaling( const Vec3f& axisScale );
	void ApplyScaling( const float uniformScale );
	void ApplyPerspectiveProjection( const float fovy, const float aspect, const float zNear, const float zFar );
	void ApplyOrthoProjection( const float left, const float right, const float bottom, const float top, const float zNear, const float zFar );

	float* GetCurrentMatrixData();

	void InitializeAdvancedRenderingFunctions();


private:
	void Initialize();

private:
	std::vector< Matrix4 >	m_matrixStack;
};

extern std::vector< DebugDrawObject* >	g_drawObjects;
extern VennRenderer*	g_theRenderer;


//---------------------------------------------------------------------------
inline void VennRenderer::PushMatrix()
{
	//glPushMatrix();
	m_matrixStack.push_back( m_matrixStack.back() );
}


//---------------------------------------------------------------------------
inline void VennRenderer::PopMatrix()
{
	//glPopMatrix();
	if ( m_matrixStack.size() > 1 )
	{
		m_matrixStack.pop_back();
	}
	else
	{
		LoadIdentityMatrix();
	}
}


//---------------------------------------------------------------------------
inline void VennRenderer::LoadIdentityMatrix()
{
	//glLoadIdentity();
	m_matrixStack.back().SetToIdentity();
}


//---------------------------------------------------------------------------
inline void VennRenderer::ApplyScaling( const Vec3f& axisScale )
{
	Matrix4 scaleMatrix = Matrix4::GetScaleMatrix( axisScale );

	//glScalef( axisScale.x, axisScale.y, axisScale.z );
	m_matrixStack.back().ApplyTransformationMatrix( scaleMatrix );
}


//---------------------------------------------------------------------------
inline void VennRenderer::ApplyScaling( const float uniformScale )
{
	Matrix4 scaleMatrix = Matrix4::GetScaleMatrix( uniformScale );

	//glScalef( uniformScale, uniformScale, uniformScale );
	m_matrixStack.back().ApplyTransformationMatrix( scaleMatrix );
}


//---------------------------------------------------------------------------
inline void VennRenderer::ApplyTranslation( const Vec3f& translation )
{
	Matrix4 translationMatrix = Matrix4::GetTranslationMatrix( translation );

	//glTranslatef( translation.x, translation.y, translation.z );
	m_matrixStack.back().ApplyTransformationMatrix( translationMatrix );
}


//---------------------------------------------------------------------------
inline void VennRenderer::ApplyRotation( const Degrees angle, const Vec3f& rotationAxis )
{
	Matrix4 rotationMatrix = Matrix4::GetRotationMatrix( angle, rotationAxis );

	//glRotatef( angle, rotationAxis.x, rotationAxis.y, rotationAxis.z );
	m_matrixStack.back().ApplyTransformationMatrix( rotationMatrix );
}


//---------------------------------------------------------------------------
inline void VennRenderer::ApplyOrthoProjection( const float left, const float right, const float bottom, const float top, const float zNear, const float zFar )
{
	LoadIdentityMatrix();
	m_matrixStack.back().SetOrtho( left, right, bottom, top, zNear, zFar );
// 	Matrix4& currentMatrix = m_matrixStack.back();
// 	Vec3f oneOverVals( 1.f / ( right - left ), 1.f / ( top - bottom ), 1.f / ( zFar - zNear ) );
// 	Vec3f orthoScale( 2.f * oneOverVals.x, 2.f * oneOverVals.y, -2.f * oneOverVals.z );
// 	Vec3f orthoTranslate( - ( right + left ) * oneOverVals.x, - ( top + bottom ) * oneOverVals.y, - ( zFar + zNear ) * oneOverVals.z );
// 
// 	Matrix4 translateMatrix = Matrix4::GetTranslationMatrix( orthoTranslate );
// 	Matrix4 scaleMatrix = Matrix4::GetScaleMatrix( orthoScale );
// 
// 	currentMatrix.ApplyTransformationMatrix( translateMatrix );
// 	currentMatrix.ApplyTransformationMatrix( scaleMatrix );
}


//---------------------------------------------------------------------------
inline void VennRenderer::ApplyPerspectiveProjection( const float fovy, const float aspect, const float zNear, const float zFar )
{
	LoadIdentityMatrix();
	//glLoadIdentity();
	//gluPerspective( fovy, aspect, zNear, zFar );
	Matrix4& currentMatrix = m_matrixStack.back();
// 	float f = 1.f / tan( fovy / 2.f );
// 	float oneOverZDifference = 1.f / ( zNear - zFar );
// 
// 	Vec3f perspectiveScale( f / aspect, f, ( zFar + zNear ) / oneOverZDifference );
// 	Vec3f perspectiveTranslate( 0.f, 0.f, ( 2.f * zFar * zNear ) / oneOverZDifference );

// 	Matrix4 translationMatrix = Matrix4::GetTranslationMatrix( perspectiveTranslate );
// 	Matrix4 scaleMatrix = Matrix4::GetScaleMatrix( perspectiveScale );

	currentMatrix.SetPerspective( fovy, aspect, zNear, zFar );
// 	currentMatrix.ApplyTransformationMatrix( translationMatrix );
// 	currentMatrix.ApplyTransformationMatrix( scaleMatrix );
}


//---------------------------------------------------------------------------
inline float* VennRenderer::GetCurrentMatrixData()
{
	return m_matrixStack.back().GetMatrixData();
}


#endif