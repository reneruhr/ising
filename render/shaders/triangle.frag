#version 410  core

uniform int width;
uniform float line_thickness;

out vec4 color;

void main()
{
	vec2 uv = gl_FragCoord.xy/width;
	uv.x -= 0.5;
	uv *= 2.;
	float l = dot(uv,uv);

	if(   l < 1) discard;
	else if(  ( l > 1.+line_thickness  )
	   && abs(abs(uv.x)-0.5) > line_thickness/3 ) discard; 
	color = vec4(1.,1.,0.,1.);
}
