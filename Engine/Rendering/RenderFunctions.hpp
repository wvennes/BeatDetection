//---------------------------------------------------------------------------
// RenderFunctions.hpp
// * OpenGL convenience functions
// NOTE: Assumes textures are bound and transforms are done beforehand
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_RENDERFUNCTIONS_
#define _INCLUDED_RENDERFUNCTIONS_

#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Rendering/VennRenderer.hpp"

class VennRenderer;
struct Vertex3D;

void DrawQuad();
void DrawQuadColor( const Vec3f& color );
void DrawQuadWithShader( const unsigned int shaderProgramID );
void DrawLineQuad();
void DrawTexturedQuad();
void DrawTexturedQuadFromSpriteSheet( const Vec2f& minTexCoords, const Vec2f& maxTexCoords );

void DrawQuadAboutCenter();
void DrawTexturedQuadAboutCenter();
void DrawTexturedQuadFromSpriteSheetAboutCenter( const Vec2f& minTexCoords, const Vec2f& maxTexCoords );

void DrawAxisAtCenter();
void DrawAxisAtCenter( unsigned int shaderProgramID );

void DrawTexturedCube( const Vec2f& minTexCoords, const Vec2f& maxTexCoords );

void RenderVertexArrayWithShader( const Vertex3D* vertices, int arraySize, unsigned int shaderProgramID, VennEnum mode );
void RenderVertexArrayWithShaderAndNormals( const Vertex3D* vertices, const int arraySize, const unsigned int shaderProgramID, const VennEnum mode );
void RenderVertexArray( const Vertex3D* vertices, const int arraySize, const VennEnum mode );
void RenderVBO( const unsigned int vboID, const int arraySize, const VennEnum mode );
void RenderVBOWithShaderAndNormals( const unsigned int vboID, const int arraySize, const unsigned int shaderProgramID, const VennEnum mode );
void RenderElements( const Vertex3D* vertices, const int arraySize, const int* indices, const int indexCount, const VennEnum mode );
void RenderElementsWithVBOAndIBO( const unsigned int vboID, const unsigned int iboID, const unsigned int indexCount, const unsigned int shaderProgramID, const VennEnum mode );

extern VennRenderer* g_theRenderer;

#endif