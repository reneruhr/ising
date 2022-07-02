#version 410  core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 col;

uniform mat4 mvp;
vec3 out = vert_col;

void main()
{
	gl_Position = mvp*vec4(pos,0.f,1.f);
	gl_PointSize = 8.;
	vert_col = col;
}