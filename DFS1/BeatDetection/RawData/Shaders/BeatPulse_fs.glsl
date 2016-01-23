#version 330

uniform float	u_currentTime;
uniform float	u_maxTime;										
															
in vec4			v_color;
in vec2			v_texCoords;
															
out vec4		o_fragColor;

//uniform sampler2D u_diffuseMap;								

void main( void )
{
	//vec4 texColor = texture2D( u_diffuseMap, v_texCoords );
	float percentage = u_currentTime / u_maxTime;	
	o_fragColor = mix( v_color, vec4( 1.0, 1.0, 1.0, 1.0 ), percentage );	
	//o_fragColor = vec4( 1.0, 0.0, 0.0, 1.0 );					
}