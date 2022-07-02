#version 410  core

layout (location = 0) in vec2 pos;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp*vec4(pos,0.f,1.f);
	gl_PointSize = 8.;
}
