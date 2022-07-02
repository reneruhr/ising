#include "shader.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../tools.h"

namespace Render
{

Shader::Shader(std::string vert_name, std::string frag_name)
{
	vert_shader = glCreateShader(GL_VERTEX_SHADER);	
	auto  vert_shader_source = Tools::LoadFile(vert_name); 
	auto  vert_shader_source_ptr = vert_shader_source.c_str();
	std::cout << "Loaded Shader:\n" << vert_shader_source_ptr;
	glShaderSource(vert_shader, 1, &vert_shader_source_ptr , nullptr);
	glCompileShader(vert_shader);

	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);	
	auto  frag_shader_source = Tools::LoadFile(frag_name); 
	auto  frag_shader_source_ptr =frag_shader_source.c_str();
	std::cout << "Loaded Shader:\n" << frag_shader_source_ptr;
	glShaderSource(frag_shader, 1, &frag_shader_source_ptr,  nullptr);
	glCompileShader(frag_shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vert_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	program_ = glCreateProgram();
	glAttachShader(program_, vert_shader);
	glAttachShader(program_, frag_shader);
	glLinkProgram(program_);

	glGetProgramiv(program_, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(program_, 512, NULL, infoLog);
		std::cout << "ERROR with Program\n" << infoLog << std::endl;	
	}

	glDeleteShader(frag_shader);
	glDeleteShader(vert_shader);
}

}
