// shadertype=glsl

#version 330													
																
uniform mat4	u_worldToClipSpaceMatrix;		
																
in vec3			a_position;												
in vec4			a_color;												
in vec2			a_texCoords;											
																
out vec4		v_color;												
out vec2		v_texCoords;											
																
void main( void )												
{																
	vec4 vertexPosition = vec4( a_position, 1.f );

	v_color = a_color;
	v_texCoords = a_texCoords;
	gl_Position = u_worldToClipSpaceMatrix * vertexPosition;
}