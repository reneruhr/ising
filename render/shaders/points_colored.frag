#version 410  core

out vec4 frag_color; 
in vec3 vert_color;

void main()
{
	frag_color = vec4(vert_color ,1.);
}
