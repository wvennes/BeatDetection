// shadertype=glsl

#version 330													
																
uniform mat4	u_worldToClipSpaceMatrix;
uniform float	u_currentTime;
uniform float	u_maxTime;
uniform float	u_scale;		
																
in vec3			a_position;												
in vec4			a_color;												
in vec2			a_texCoords;											
																
out vec4		v_color;												
out vec2		v_texCoords;											
																
void main( void )												
{																
	vec4 vertexPosition = vec4( a_position, 1.f );
	vertexPosition.xy = vertexPosition.xy * ( 1.0 + ( u_currentTime / u_maxTime ) * u_scale );
	//vertexPosition.y *= ( 1.0 + u_time );
	//mat4 worldToClipBasedOnTime = u_worldToClipSpaceMatrix;
	//worldToClipBasedOnTime[ 0 ][ 0 ] *= ( 1.0 + u_time );
	//worldToClipBasedOnTime[ 1 ][ 1 ] *= ( 1.0 + u_time );
	//worldToClipBasedOnTime[ 2 ][ 2 ] *= ( 1.0 + u_time );
	v_color = a_color;
	v_texCoords = a_texCoords;
	gl_Position = u_worldToClipSpaceMatrix * vertexPosition;
}