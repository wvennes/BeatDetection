#version 330	

uniform float	u_time;											
															
in vec4			v_color;											
in vec2			v_texCoords;										
															
out vec4		o_fragColor;										
								
void main( void )											
{
	vec4 colorOverTime;
	colorOverTime.r = 1.8 * abs( sin( u_time ) ) + cos( u_time );
	colorOverTime.g = abs( cos( u_time ) );
	colorOverTime.b = abs( sin( u_time ) );

	o_fragColor = colorOverTime * v_color;	
}