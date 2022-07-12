#include "render.h"
#include "../tools/tools.h"
#include "shader.h"
#include "buffer.h"
#include "colors.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <array>


namespace Render 
{

auto DrawTriangle() -> std::function<void(int, float)> 
{
	auto shader = Shader("render/shaders/circle.vert", "render/shaders/triangle.frag");
	auto DrawCall = SquareBuffer();
	
	return [p = shader.Id(), Draw = DrawCall](int w, float t)
	{
		glUseProgram(p);	
		glUniform1i( glGetUniformLocation(p, "width"), w);
		glUniform1f( glGetUniformLocation(p, "line_thickness"), t);
		Draw();
	};
}

auto DrawPoints(const Buffer& buffer) -> std::function<void(MVP, Color)>
{
	auto shader = Shader("rende/shaders/points.vert", "render/shaders/points.frag");
	auto DrawCall = PointsBuffer(buffer);
	
	glEnable(GL_PROGRAM_POINT_SIZE);

	return [p = shader.Id(), Draw = DrawCall](MVP mvp, Color color)
	{
		glUseProgram(p);	
		glUniformMatrix4fv(glGetUniformLocation(p, "mvp"), 1, GL_FALSE, mvp.data());
		glUniform3fv( glGetUniformLocation(p, "color"), 1, MakeRGB(color).data());
		Draw();
	};
}

auto DrawColoredPoints(const Buffer& buffer) -> std::function<void(MVP,float)>
{
	auto shader = Shader("render/shaders/points_colored.vert", "render/shaders/points_colored.frag");
	auto DrawCall = PointsBuffer(buffer);
	
	glEnable(GL_PROGRAM_POINT_SIZE);

	return [p = shader.Id(), Draw = DrawCall](MVP mvp, float point_size)
	{
		glUseProgram(p);	
		glUniformMatrix4fv(glGetUniformLocation(p, "mvp"), 1, GL_FALSE, mvp.data());
		glUniform1f(glGetUniformLocation(p, "point_size"), point_size);
		Draw();
	};
}
  
}
