#pragma once
#include <string>


namespace Render
{
class Shader
{
	unsigned int vert_shader, frag_shader;
	unsigned int program_;

public:
	Shader(std::string, std::string);
	
	auto Id() -> unsigned int { return program_; }	
};
}
