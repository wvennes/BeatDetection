//---------------------------------------------------------------------------
// BuiltInShaders.hpp
// -- Default shaders for default material generation
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_BUILTINSHADERS_
#define _INCLUDED_BUILTINSHADERS_


const char* unlitVertex = 
"																	\n\
	#version 330													\n\
																	\n\
	uniform mat4 u_worldToClipSpaceMatrix;							\n\
																	\n\
	in vec3 a_position;												\n\
	in vec4 a_color;												\n\
	in vec2 a_texCoords;											\n\
																	\n\
	out vec4 v_color;												\n\
	out vec2 v_texCoords;											\n\
																	\n\
	void main( void )												\n\
	{																\n\
		vec4 vertexPosition = vec4( a_position, 1.f );				\n\
		v_color = a_color;											\n\
		v_texCoords = a_texCoords;									\n\
		gl_Position = u_worldToClipSpaceMatrix * vertexPosition;	\n\
	}																\n\
\0";


const char* unlitFragment = 
"																	\n\
		#version 330												\n\
																	\n\
		in vec4 v_color;											\n\
		in vec2 v_texCoords;										\n\
																	\n\
		out vec4 o_fragColor;										\n\
																	\n\
		uniform sampler2D u_diffuseMap;								\n\
																	\n\
		void main( void )											\n\
		{															\n\
			vec4 texColor = texture2D( u_diffuseMap, v_texCoords );	\n\
			o_fragColor = v_color * texColor;						\n\
		}															\n\
\0";

#endif